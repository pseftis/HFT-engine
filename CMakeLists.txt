cmake_minimum_required(VERSION 3.10)
project(HFTEngine)

set(CMAKE_CXX_STANDARD 17)

# Linking Winsock for Windows Network Testing
if(WIN32)
    link_libraries(ws2_32)
endif()

include_directories(src)

add_library(hft_core
    src/simulator/exchange_sim.cpp
    src/engine/oms.cpp
    src/engine/risk_manager.cpp
    src/engine/strategy.cpp
)

add_executable(hft_engine src/main.cpp)
target_link_libraries(hft_engine hft_core)
