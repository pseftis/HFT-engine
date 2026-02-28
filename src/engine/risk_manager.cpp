#include "risk_manager.hpp"

RiskManager::RiskManager(int max_pos, double max_loss) : max_position(max_pos), max_daily_loss(max_loss), kill_switch(false) {}

bool RiskManager::check_order(const Order& order, std::string& reject_reason) {
    if (kill_switch) {
        reject_reason = "Kill switch activated";
        return false;
    }
    if (order.qty > max_position) {
        reject_reason = "Order exceeds max position size";
        return false;
    }
    return true;
}

void RiskManager::activate_kill_switch() { kill_switch = true; }
