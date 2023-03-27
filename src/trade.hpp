#ifndef TRADE_HPP
#define TRADE_HPP

#include <vector>

#include "order.hpp"

struct Trade {
    OrderID buy_order_id;
    OrderID sell_order_id;
    double price;
    double quantitiy;
};

typedef std::vector<Trade> TradeBook;

# endif // TRADE_HPP
