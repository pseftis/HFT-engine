#pragma once
#include "../common/types.hpp"
#include <functional>

class ExchangeSim {
public:
    using ExecCallback = std::function<void(const ExecutionReport&)>;
    ExchangeSim(ExecCallback cb);
    void set_callback(ExecCallback cb);
    void set_latency(uint64_t micros);
    void handle_order(Order& order);
    void cancel_order(uint64_t id);

private:
    ExecCallback callback;
    uint64_t artificial_latency_micros;
};
