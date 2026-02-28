#pragma once
#include <chrono>
#include <cstdint>

inline uint64_t current_micros() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}
