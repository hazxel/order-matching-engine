#include "engine.hpp"

Engine::Engine() {
    next_order_id_ = 0;
    for (Instrument i = FIRST_INS; i != LAST_INS; ++i) {
        open_order_books_[i] = {BuyOrderBook(), SellOrderBook()};
        executed_trades_[i] = TradeBook();
        previous_transaction_price_[i] = 0;
    }
    is_running_ = true;
    insert_order_thread_ = std::thread(&Engine::insert_order, this);
}

Engine::~Engine() {
    {
        std::unique_lock<std::mutex> lock(input_queue_mutex_);
        is_running_ = false;
    }
    input_cv_.notify_one();
    if (insert_order_thread_.joinable()) {
        insert_order_thread_.join();
    }
}

void Engine::place_order(Instrument ins, OrderSide side, double quant, double price) {
    std::thread t([=](){
        {
            std::unique_lock<std::mutex> lock(input_queue_mutex_);
            input_queue_.emplace(ins, side, quant, price);
        }
        input_cv_.notify_one();
    });
    t.detach();   
}

OutputConfirm Engine::get_confirm() {
    if (output_queue_.empty()) {
        return {-1, CANCELED};
    }
    std::unique_lock<std::mutex> lock(output_queue_mutex_);
    if (output_queue_.empty()) {
        return {-1, CANCELED};
    }
    OutputConfirm oc = output_queue_.front();
    output_queue_.pop();
    return oc;
}


void Engine::cancel_order(OrderID id) {
    std::thread t([id, this](){
        std::unique_lock<std::mutex> lock(order_book_mutex_);
        remove_order(order_dict_[id]);
    });
}

void Engine::remove_order(const Order &order) {
    OrderID id = order.id;
    if (order_dict_.find(id) == order_dict_.end()) {
        return;
    }
    if (order.side == BUY) {
        open_order_books_[order.instrument].buy.erase(order);    
    } else {
        open_order_books_[order.instrument].sell.erase(order);    
    }
    order_dict_.erase(id);
}

void Engine::insert_order() {
    while (true) {
        OrderID id;
        {
            std::unique_lock<std::mutex> input_queue_lock(input_queue_mutex_);
            input_cv_.wait(input_queue_lock, [this](){ return !input_queue_.empty() || !is_running_; });
            if (!is_running_) {
                return;
            }

            IncomingOrder &io = input_queue_.front();
            Order order = {generate_order_id(), io.ins, io.side, io.price, io.quant};

            std::unique_lock<std::mutex> order_book_lock(order_book_mutex_);
            if (io.side == BUY) {
                open_order_books_[order.instrument].buy.insert(order);    
            } else {
                open_order_books_[order.instrument].sell.insert(order);    
            }
            order_dict_[order.id] = order;

            id = order.id;
            input_queue_.pop();
        }
        insert_confirm(id, OPEN);
        match_order(order_dict_[id]);
    }
}

void Engine::match_order(Order &order) {
    Instrument ins = order.instrument;
    OrderBook &order_book = open_order_books_[ins];

    std::unique_lock<std::mutex> order_book_lock(order_book_mutex_);
    if (order.side == BUY ? order_book.sell.empty() : order_book.buy.empty()) {
        // no order to match
        return;
    }
    const Order &target_order = order.side == BUY ? *order_book.sell.begin() : *order_book.buy.begin();
    double price = calc_price(target_order, order);

    if (price < 0) {
        // no match
        return;
    }

    if (order.quantity > target_order.quantity) {
        if (order.side == BUY) {
            order_book.buy.erase(order);
            order.quantity -= target_order.quantity;
            order_book.buy.insert(order);
        } else {
            order_book.sell.erase(order);
            order.quantity -= target_order.quantity;
            order_book.sell.insert(order);
        }
        insert_confirm(order.id, PARCIAL_EXECUTED);
        insert_confirm(target_order.id, EXECUTED);
        executed_trades_[ins].emplace_back(
            order.side == BUY ? order.id : target_order.id, 
            order.side == SELL ? order.id : target_order.id, 
            price, 
            target_order.quantity, 
            ins);
        remove_order(target_order);
        order_book_lock.unlock();
        previous_transaction_price_[ins] = price;
        match_order(order);
    } else if (order.quantity < target_order.quantity) {
        insert_confirm(order.id, EXECUTED);
        insert_confirm(target_order.id, PARCIAL_EXECUTED);
        order_dict_[target_order.id].quantity -= order.quantity;
        if (target_order.side == BUY) {
            order_book.buy.erase(target_order);
            order_book.buy.emplace(
                target_order.id, 
                target_order.instrument,
                target_order.side,
                target_order.price,
                target_order.quantity - order.quantity);
        } else {
            order_book.sell.erase(target_order);
            order_book.sell.emplace(
                target_order.id, 
                target_order.instrument,
                target_order.side,
                target_order.price,
                target_order.quantity - order.quantity);
        }
        executed_trades_[ins].emplace_back(
            order.side == BUY ? order.id : target_order.id, 
            order.side == SELL ? order.id : target_order.id, 
            price, 
            order.quantity, 
            ins);
        remove_order(order);
        previous_transaction_price_[ins] = price;
    } else {
        insert_confirm(order.id, EXECUTED);
        insert_confirm(target_order.id, EXECUTED);
        executed_trades_[ins].emplace_back(
            order.side == BUY ? order.id : target_order.id, 
            order.side == SELL ? order.id : target_order.id, 
            price, 
            order.quantity, 
            ins);
        remove_order(order);
        remove_order(target_order);
        previous_transaction_price_[ins] = price;
    }
}

double Engine::calc_price(const Order &buy_order, const Order &sell_order) {
    if (buy_order.side == sell_order.side || buy_order.instrument != sell_order.instrument) {
        return -1;
    }
    if (buy_order.side != BUY) {
        return calc_price(sell_order, buy_order);
    }
    if (buy_order.price < sell_order.price) {
        return -1;
    } else if (buy_order.price > sell_order.price) {
        double prev_price = previous_transaction_price_[buy_order.instrument];
        if (prev_price > buy_order.price) {
            return buy_order.price;
        } else if (prev_price < sell_order.price) {
            return sell_order.price;
        } else {
            return prev_price;
        }
    } else {
        return buy_order.price;
    }
}

void Engine::insert_confirm(OrderID id, OrderStatus os) {
    std::unique_lock<std::mutex> lock(output_queue_mutex_);
    output_queue_.emplace(id, os);
}