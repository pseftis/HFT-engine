#include "exchange_sim.hpp"
#include "../common/time_utils.hpp"

ExchangeSim::ExchangeSim(ExecCallback cb) : callback(cb), artificial_latency_micros(0) {}

void ExchangeSim::set_callback(ExecCallback cb) { callback = cb; }
void ExchangeSim::set_latency(uint64_t micros) { artificial_latency_micros = micros; }

void ExchangeSim::handle_order(Order& order) {
    if (artificial_latency_micros > 0) {
        volatile uint64_t start = current_micros();
        while(current_micros() - start < artificial_latency_micros) {}
    }
    ExecutionReport report;
    report.order_id = order.id;
    report.latency_micros = current_micros() - order.creation_time;

    if (order.type == OrderType::MARKET) {
        report.status = OrderStatus::FILLED;
        report.filled_qty = order.qty;
        report.fill_price = order.price;
        order.status = OrderStatus::FILLED;
        order.filled_qty = order.qty;
    } else {
        if (order.qty > 100) {
            report.status = OrderStatus::PARTIALLY_FILLED;
            report.filled_qty = 100;
            report.fill_price = order.price;
            order.status = OrderStatus::PARTIALLY_FILLED;
            order.filled_qty += 100;
        } else {
            report.status = OrderStatus::FILLED;
            report.filled_qty = order.qty - order.filled_qty;
            report.fill_price = order.price;
            order.status = OrderStatus::FILLED;
            order.filled_qty = order.qty;
        }
    }
    if (callback) callback(report);
}

void ExchangeSim::cancel_order(uint64_t id) {
    ExecutionReport report;
    report.order_id = id;
    report.status = OrderStatus::CANCELED;
    report.filled_qty = 0;
    report.fill_price = 0.0;
    report.message = "Canceled";
    report.latency_micros = 10;
    if (callback) callback(report);
}
