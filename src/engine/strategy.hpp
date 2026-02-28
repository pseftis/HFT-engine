#pragma once
#include "oms.hpp"
#include "../common/types.hpp"

class Strategy {
public:
    Strategy(OMS* o);
    void on_market_data(const Tick& tick);

private:
    OMS* oms;
    uint64_t order_id_counter;
};
