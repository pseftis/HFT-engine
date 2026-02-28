#include "oms.hpp"
#include "../common/time_utils.hpp"

OMS::OMS(RiskManager* risk, ExchangeSim* exch, std::function<void(const std::string&)> out_cb) 
    : risk_mgr(risk), exchange(exch), output_cb(out_cb) {
    exchange_cb = [this](const ExecutionReport& rep) {
        if (output_cb) output_cb(rep.to_string());
    };
    exch->set_callback(exchange_cb);
}

void OMS::place_order(uint64_t id, const std::string& sym, Side side, OrderType type, double px, int qty) {
    Order o{id, sym, side, type, px, qty, 0, OrderStatus::NEW, current_micros()};
    std::string reject_msg;
    if (!risk_mgr->check_order(o, reject_msg)) {
        ExecutionReport rej{id, OrderStatus::REJECTED, 0, 0.0, reject_msg, current_micros() - o.creation_time};
        if (output_cb) output_cb(rej.to_string());
        return;
    }
    orders[id] = o;
    exchange->handle_order(orders[id]);
}

void OMS::cancel_order(uint64_t id) {
    if (orders.find(id) != orders.end()) {
        exchange->cancel_order(id);
    }
}
