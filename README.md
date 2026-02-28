# High-Frequency Trading (HFT) Engine Simulator

A simulated High-Frequency Trading (HFT) system designed to execute orders, simulate market latency, enforce risk limits, and run automated algorithmic strategies over a raw TCP socket. 

This project aims to mimic the system architecture of a modern quantitative trading firm, built with **C++17** for the high-performance core and **Python/Pytest** for the testing and automation layer.

## 🏗 System Architecture

The project is divided into two primary layers:

### 1. Trading Engine Core (C++)
The internal engine process listens on a raw TCP socket (Runs on Port `8080`) routing nanosecond-scale execution logic:
- **Order Management System (OMS)**: Maps and keeps state of all live orders, partial fills, and cancellations.
- **Risk Manager**: Before an order hits the matching engine, it passes through risk bounds (e.g., maximum position sizes) or can be blocked entirely via a software *Kill Switch*. 
- **Strategy Module**: Currently runs a simulated algorithmic spread-capture strategy. It ingests Market Data ticks and automatically places tight bounds based on spread differentials.
- **Exchange Simulator**: A venue matching engine mimicking the real-world. It supports instantaneous execution, handles remainder partial fills, and can artificially inject exact microsecond latency to measure algorithmic bottlenecks.

### 2. Testing Framework (Python)
An automated Python suite acts as the network client, verifying functionality and proving latency profiles:
- **Functional Testing**: Asserts that partial fills correctly decrement quantities, order cancellation signals halt execution, and risk filters aggressively reject oversized trades.
- **Performance Profiling**: Validates network throughput by slamming the C++ engine over TCP, proving its ability to process thousands of orders concurrently dynamically tracking simulated execution latency.

---

## 🚀 Getting Started (Windows MVS)

### Requirements:
- Windows OS
- CMake (`3.10+`)
- Visual Studio / MSVC Build Tools
- Python 3+

### Building & Running the Suite
The repository includes an all-in-one PowerShell script that:
1. Builds the C++ Core Engine via CMake.
2. Creates a Python Virtual Environment (`venv`).
3. Installs testing dependencies (`pytest`, `socket` tooling).
4. Boots the compiled binary and runs the full test suite in real-time.

```powershell
# Navigate into the project directory
cd hft_engine

# Run the automated build and test orchestrator
.\build_and_run.ps1
```

## 🧪 Testing Scope
The `tests/` directory uses `pytest` and a custom non-blocking TCP Python wrapper to simulate real market conditions asynchronously.
- `test_functional.py` - Core logic mapping, Order Cancellations, Partial fill math.
- `test_risk.py` - Kill switch intercepts and max quantity safety filters.
- `test_performance.py` - Inundates the core engine measuring absolute latency (`µs`) and generic throughput (`orders/sec`).
- `test_strategy.py` - Validates the auto-generated Strategy algorithms reacting dynamically to injected Market Data.
