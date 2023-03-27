#ifndef ORDER_HPP
#define ORDER_HPP

#include <functional>
#include <set>

typedef long OrderID;

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderStatus {
    OPEN,
    EXECUTED,
    CANCELLED
};

enum class Instrument {
    FIRST_INS,
    BTC = FIRST_INS,
    ETH,
    USDT,
    LAST_INS
};

inline const Instrument operator++ (Instrument& i) {
    if (i == Instrument::LAST_INS) {
        return Instrument::FIRST_INS;
    }
    return static_cast<Instrument>(static_cast<int>(i) + 1);
}

inline Instrument & operator++ (Instrument& i, int) {
    if (i != Instrument::LAST_INS) {
        i = static_cast<Instrument>(static_cast<int>(i) + 1);
    }
    return i;
}

struct Order {
    OrderID order_id;
    Instrument instrument;
    OrderSide side;
    double price;
    double quantity;
    OrderStatus status;
};

static auto order_price_comparator = [](const Order& o1, const Order& o2) {
    if (o1.price != o2.price) {
        return o1.price < o2.price;
    }
    return o1.order_id < o2.order_id;
};

typedef std::multiset<Order, decltype(order_price_comparator)> OrderBook;

# endif // ORDER_HPP
