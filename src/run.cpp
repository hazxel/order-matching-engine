#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"
#include "engine.hpp"


std::string ins2str(Instrument i) {
    switch (i) {
        case BTC: return "BTC";
        case ETH: return "ETH";
        case USDT: return "USDT";
        default: return "UNKNOWN";
    }
}

std::string side2str(OrderSide s) {
    switch (s) {
        case BUY: return "BUY";
        case SELL: return "SELL";
        default: return "UNKNOWN";
    }
}

std::string status2str(OrderStatus s) {
    switch (s) {
        case OPEN: return "OPEN";
        case EXECUTED: return "EXECUTED";
        case PARCIAL_EXECUTED: return "PARCIAL_EXECUTED";
        case CANCELED: return "CANCELED";
        default: return "UNKNOWN";
    }
}

void place_order(Engine &engine, int thread_id, Instrument ins, OrderSide side, double quantity, double price) {
    engine.place_order(ins, side, quantity, price);
    std::cout << "thread: " << thread_id << ", instrument: " << ins2str(ins) << ", side: " << side2str(side) << ", price: " << price << ", quantity: " << quantity << std::endl;
}

int main() {
    Engine engine;

    std::cout << "Placing orders: " << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 1000; i++) {
        threads.emplace_back([&engine, i]() {
            place_order(engine, i, BTC, OrderSide::BUY, 5, 1);
            place_order(engine, i, BTC, OrderSide::SELL, 1, 1);
            place_order(engine, i, BTC, OrderSide::SELL, 1, 1);
            place_order(engine, i, BTC, OrderSide::SELL, 7, 0.1);
        });
    }
    for (auto &t : threads) {
        t.join();
    }

    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << std::endl << "Remaining orders: " << std::endl;
    OrderBook ob = engine.get_open_orders(BTC);
    std::cout << "order book buy size: " << ob.buy.size() << std::endl;
    for (auto o : ob.buy) {
        std::cout << " - id: " << o.id << ", price: " << o.price << ", quantity: " << o.quantity << std::endl;
    }
    std::cout << "order book sell size: " << ob.sell.size() << std::endl;
    for (auto o : ob.sell) {
        std::cout << " - id: " << o.id << ", price: " << o.price << ", quantity: " << o.quantity << std::endl;
    }

    std::cout << std::endl << "Confirmations: " << std::endl;
    OutputConfirm oc = engine.get_confirm();
    while (oc.id >= 0) {
        std::cout << " - id: " << oc.id << " status: " << status2str(oc.status) << std::endl;
        oc = engine.get_confirm();
    }

    std::cout << std::endl << "Executed trades: " << std::endl;
    TradeBook tb = engine.get_executed_trades(BTC);
    for (auto t : tb) {
        std::cout << " - buy_id: " << t.buy_order_id << ", sell_id: " << t.sell_order_id << ", price: " << t.price << ", quantity: " << t.quantitiy << std::endl;
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}