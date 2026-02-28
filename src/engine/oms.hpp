#pragma once
#include "risk_manager.hpp"
#include "../simulator/exchange_sim.hpp"
#include <unordered_map>
#include <functional>
#include <string>

class OMS {
public:
    OMS(RiskManager* risk, ExchangeSim* exch, std::function<void(const std::string&)> out_cb);
    void place_order(uint64_t id, const std::string& sym, Side side, OrderType type, double px, int qty);
    void cancel_order(uint64_t id);

private:
    RiskManager* risk_mgr;
    ExchangeSim* exchange;
    std::unordered_map<uint64_t, Order> orders;
    std::function<void(const std::string&)> output_cb;
    ExchangeSim::ExecCallback exchange_cb;
};
