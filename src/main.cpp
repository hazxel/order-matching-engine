#include <iostream>
#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"
#include "engine.hpp"


int main() {
    Engine engine;
    engine.place_order(BTC, OrderSide::BUY, 5, 1);
    engine.place_order(BTC, OrderSide::SELL, 1, 1);
    engine.place_order(BTC, OrderSide::SELL, 1, 1);
    engine.place_order(BTC, OrderSide::SELL, 7, 0.1);
    // TradeBook tb = engine.get_executed_trades(BTC);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    OrderBook ob = engine.get_open_orders(BTC);
    std::cout << "order book buy size: " << ob.buy.size() << std::endl;
    for (auto o : ob.buy) {
        std::cout << o.id << " " << o.price << " " << o.quantity << std::endl;
    }
    std::cout << "order book sell size: " << ob.sell.size() << std::endl;
    for (auto o : ob.sell) {
        std::cout << o.id << " " << o.price << " " << o.quantity << std::endl;
    }

    OutputConfirm oc = engine.get_confirm();
    while (oc.id >= 0) {
        std::cout << "confirm: " << oc.id << " status: " << oc.status << std::endl;
        oc = engine.get_confirm();
    }

    std::cout << "ending..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}