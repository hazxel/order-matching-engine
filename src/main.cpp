#include <iostream>
#include <unordered_map>

#include "order.hpp"
#include "trade.hpp"
#include "engine.hpp"


int main() {
    Engine engine;
    engine.place_order(BTC, OrderSide::BUY, 1 ,1);
    TradeBook tb = engine.get_executed_trades(BTC);
    OrderBook ob = engine.get_open_orders(BTC);
    // for (auto o : engine.get_open_orders(BTC)) {
    //     std::cout << o.price << " " << o.quantity << std::endl;
    // }
    return 0;
}