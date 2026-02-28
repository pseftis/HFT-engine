#pragma once
#include <string>
#include <cstdint>
#include "../common/types.hpp"

class RiskManager {
public:
     RiskManager(int max_pos, double max_loss);
     bool check_order(const Order& order, std::string& reject_reason);
     void activate_kill_switch();

private:
     int max_position;
     double max_daily_loss; // Simplified
     bool kill_switch;
};
