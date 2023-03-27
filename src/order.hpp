#ifndef ORDER_HPP
#define ORDER_HPP

#include <functional>
#include <unordered_map>
#include <set>

typedef long OrderID;

enum OrderSide {
    BUY,
    SELL
};

enum OrderStatus {
    OPEN,
    EXECUTED,
    CANCELLED
};

enum Instrument {
    FIRST_INS,
    BTC = FIRST_INS,
    ETH,
    USDT,
    LAST_INS
};

inline const Instrument operator++ (Instrument& i) {
    if (i == LAST_INS) {
        return FIRST_INS;
    }
    return static_cast<Instrument>(static_cast<int>(i) + 1);
}

inline Instrument & operator++ (Instrument& i, int) {
    if (i != LAST_INS) {
        i = static_cast<Instrument>(static_cast<int>(i) + 1);
    }
    return i;
}

struct Order {
    OrderID id;
    Instrument instrument;
    OrderSide side;
    double price;
    double quantity;
    OrderStatus status;
};

struct OrderPriceComparator {
    bool operator()(const Order& o1, const Order& o2) const {
        if (o1.price != o2.price) {
            return o1.price < o2.price;
        }
        return o1.id < o2.id;
    }
};

typedef std::multiset<Order, OrderPriceComparator> OrderBook;
typedef std::unordered_map<OrderID, Order> OrderDict; 

# endif // ORDER_HPP
