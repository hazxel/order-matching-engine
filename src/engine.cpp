#include "engine.hpp"

Engine::Engine() {
    for (Instrument i = Instrument::FIRST_INS; i != Instrument::LAST_INS; ++i) {
        open_order_books_[i] = OrderBook(order_price_comparator);
        executed_trades_[i] = TradeBook();
    }
}

Engine::~Engine() {

}

void Engine::place_order(Order order) {

}


void Engine::cancel_order(OrderID order_id) {

}

TradeBook Engine::get_executed_trades(Instrument instrument) {
    return executed_trades_[instrument];
}

OrderBook Engine::get_open_orders(Instrument instrument) {
    return open_order_books_[instrument];
}

