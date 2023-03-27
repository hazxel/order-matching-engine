#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"

class Engine {
public:
    Engine();
    void place_order(Instrument ins, OrderSide side, double quant, double price);
    void cancel_order(OrderID order_id);
    const TradeBook & get_executed_trades(Instrument instrument);
    const OrderBook & get_open_orders(Instrument instrument);

private:
    inline OrderID generate_order_id() { return next_order_id_++; }

private:
    OrderID next_order_id_;
    OrderDict order_dict_;
    std::unordered_map<Instrument, OrderBook> open_order_books_;
    std::unordered_map<Instrument, TradeBook> executed_trades_;
};


#endif // ENGINE_HPP
