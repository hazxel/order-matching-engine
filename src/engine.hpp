#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "order.hpp"
#include "trade.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    void place_order(Instrument ins, OrderSide side, double quant, double price);
    void cancel_order(OrderID order_id);
    inline const TradeBook & get_executed_trades(Instrument instrument) { return executed_trades_[instrument]; }
    inline const OrderBook & get_open_orders(Instrument instrument) { return open_order_books_[instrument]; }
    OutputConfirm get_confirm();

private:
    inline OrderID generate_order_id() { return next_order_id_++; }
    void insert_order();
    void match_order(Order &order);
    void remove_order(const Order &order);
    void insert_confirm(OrderID id, OrderStatus os);
    double calc_price(const Order &buy_order, const Order &sell_order);

private:
    OrderID next_order_id_;

    OrderDict order_dict_;
    std::unordered_map<Instrument, OrderBook>   open_order_books_;
    std::unordered_map<Instrument, TradeBook>   executed_trades_;
    std::unordered_map<Instrument, double>      previous_transaction_price_;

    std::queue<IncomingOrder> input_queue_;
    std::queue<OutputConfirm> output_queue_;
    std::mutex input_queue_mutex_;
    std::mutex order_book_mutex_;
    std::mutex output_queue_mutex_;
    std::condition_variable input_cv_;
    std::condition_variable order_book_cv_;

    std::thread insert_order_thread_;
    bool is_running_;

};


#endif // ENGINE_HPP
