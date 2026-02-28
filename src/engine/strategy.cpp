#include "strategy.hpp"

Strategy::Strategy(OMS* o) : oms(o), order_id_counter(50000) {}

void Strategy::on_market_data(const Tick& tick) {
    double mid_price = (tick.bid + tick.ask) / 2.0;
    double spread = tick.ask - tick.bid;
    if (spread > 0.05) {
        // Place tight limit orders
        oms->place_order(order_id_counter++, tick.symbol, Side::BUY, OrderType::LIMIT, tick.bid + 0.01, 100);
        oms->place_order(order_id_counter++, tick.symbol, Side::SELL, OrderType::LIMIT, tick.ask - 0.01, 100);
    }
}
