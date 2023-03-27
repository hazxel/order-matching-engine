#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    void place_order(Order order);
    void cancel_order(OrderID order_id);
    TradeBook get_executed_trades(Instrument instrument);
    OrderBook get_open_orders(Instrument instrument);
private:
    std::unordered_map<Instrument, OrderBook> open_order_books_;
    std::unordered_map<Instrument, TradeBook> executed_trades_;
};


#endif // ENGINE_HPP
