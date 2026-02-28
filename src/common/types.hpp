#pragma once
#include <string>
#include <cstdint>
#include <sstream>

enum class Side { BUY, SELL, UNKNOWN };
enum class OrderStatus { NEW, REJECTED, PARTIALLY_FILLED, FILLED, CANCELED };
enum class OrderType { LIMIT, MARKET };

struct Order {
    uint64_t id;
    std::string symbol;
    Side side;
    OrderType type;
    double price;
    int qty;
    int filled_qty;
    OrderStatus status;
    uint64_t creation_time;
};

struct ExecutionReport {
    uint64_t order_id;
    OrderStatus status;
    int filled_qty;
    double fill_price;
    std::string message;
    uint64_t latency_micros;
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "EVT:EXEC|" << order_id << "|" << (int)status << "|" << filled_qty << "|" 
           << fill_price << "|" << latency_micros << "|" << message;
        return ss.str();
    }
};

struct Tick {
    std::string symbol;
    double bid;
    double ask;
    uint64_t timestamp;
};
