#include "engine.hpp"

Engine::Engine():next_order_id_(0) {
    for (Instrument i = FIRST_INS; i != LAST_INS; ++i) {
        open_order_books_[i] = OrderBook();
        executed_trades_[i] = TradeBook();
    }
}

void Engine::place_order(Instrument ins, OrderSide side, double quant, double price) {
    Order order = {generate_order_id(), ins, side, quant, price};
    open_order_books_[ins].insert(order);
    order_dict_[order.id] = order;
}


void Engine::cancel_order(OrderID orderId) {
    Order order = order_dict_[orderId];
    open_order_books_[order.instrument].erase(order);
    order_dict_.erase(orderId);
}