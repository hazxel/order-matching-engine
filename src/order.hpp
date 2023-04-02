#ifndef ORDER_HPP
#define ORDER_HPP

#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

typedef long OrderID;

enum OrderSide {
    BUY,
    SELL
};

enum OrderStatus {
    OPEN,
    EXECUTED,
    PARCIAL_EXECUTED,
    CANCELLED
};

enum Instrument {
    FIRST_INS,
    BTC = FIRST_INS,
    ETH,
    USDT,
    LAST_INS
};

inline Instrument & operator++ (Instrument& i) {
    if (i != LAST_INS) {
        i = static_cast<Instrument>(static_cast<int>(i) + 1);
    }
    return i;
}

inline const Instrument operator++ (Instrument& i, int) {
    Instrument temp = i;
    ++i;
    return temp;
}

struct Order {
    OrderID id;
    Instrument instrument;
    OrderSide side;
    double price;
    double quantity;
    OrderStatus status;
    // Order(OrderID i, Instrument ins, OrderSide s, double p, double q) : id(i), instrument(ins), side(s), price(p), quantity(q), status(OPEN) {}
};

struct BuyOrderPriceComparator {
    bool operator()(const Order& o1, const Order& o2) const {
        if (o1.price != o2.price) {
            return o1.price > o2.price;
        }
        return o1.id < o2.id;
    }
};

struct SellOrderPriceComparator {
    bool operator()(const Order& o1, const Order& o2) const {
        if (o1.price != o2.price) {
            return o1.price < o2.price;
        }
        return o1.id < o2.id;
    }
};

// typedef std::multiset<Order, OrderPriceComparator> OrderBook;
typedef std::unordered_map<OrderID, Order> OrderDict; 
typedef std::multiset<Order, BuyOrderPriceComparator> BuyOrderBook;
typedef std::multiset<Order, SellOrderPriceComparator> SellOrderBook;
struct OrderBook {
    BuyOrderBook buy;
    SellOrderBook sell;
};

struct IncomingOrder {
    Instrument ins;
    OrderSide side;
    double quant;
    double price;
    IncomingOrder(Instrument i, OrderSide os, double q, double p) : ins(i), side(os), quant(q), price(p) {}
};

struct OutputConfirm {
    OrderID id;
    OrderStatus status;
    OutputConfirm(OrderID i, OrderStatus s) : id(i), status(s) {}
};

# endif // ORDER_HPP
