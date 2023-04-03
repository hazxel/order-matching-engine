#ifndef TRADE_HPP
#define TRADE_HPP

#include <vector>

#include "order.hpp"

struct Trade {
    OrderID buy_order_id;
    OrderID sell_order_id;
    double price;
    double quantitiy;
    Instrument ins;
    Trade() {}
    Trade(OrderID buy_id, OrderID sell_id, double p, double q, Instrument i) : buy_order_id(buy_id), sell_order_id(sell_id), price(p), quantitiy(q), ins(i) {}
};

typedef std::vector<Trade> TradeBook;

# endif // TRADE_HPP
