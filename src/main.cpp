#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include "common/types.hpp"
#include "common/time_utils.hpp"
#include "engine/risk_manager.hpp"
#include "simulator/exchange_sim.hpp"
#include "engine/oms.hpp"
#include "engine/strategy.hpp"

#pragma comment (lib, "Ws2_32.lib")

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    std::cout << "Engine listening on port 8080...\n";

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) continue;
        std::cout << "Client connected.\n";

        auto send_msg = [&client](const std::string& msg) {
            std::string nmsg = msg + "\n";
            send(client, nmsg.c_str(), nmsg.length(), 0);
        };

        RiskManager risk(500, 1000.0);
        ExchangeSim exch(nullptr);
        OMS oms(&risk, &exch, send_msg);
        Strategy strategy(&oms);

        char buffer[4096];
        std::string remnants = "";
        while (true) {
            int bytes = recv(client, buffer, sizeof(buffer)-1, 0);
            if (bytes <= 0) break;
            buffer[bytes] = '\0';
            std::string data = remnants + buffer;
            
            size_t pos_nl = 0;
            while ((pos_nl = data.find('\n')) != std::string::npos) {
                std::string line = data.substr(0, pos_nl);
                data = data.substr(pos_nl + 1);
                
                auto parts = split(line, '|');
                if (parts.empty()) continue;
                
                if (parts[0] == "MARKET_DATA" && parts.size() >= 4) {
                    Tick t{parts[1], std::stod(parts[2]), std::stod(parts[3]), current_micros()};
                    strategy.on_market_data(t);
                } else if (parts[0] == "PLACE_ORDER" && parts.size() >= 7) {
                    uint64_t id = std::stoull(parts[1]);
                    Side side = std::stoi(parts[3]) == 0 ? Side::BUY : Side::SELL;
                    OrderType type = std::stoi(parts[4]) == 0 ? OrderType::LIMIT : OrderType::MARKET;
                    double px = std::stod(parts[5]);
                    int qty = std::stoi(parts[6]);
                    oms.place_order(id, parts[2], side, type, px, qty);
                } else if (parts[0] == "CANCEL_ORDER" && parts.size() >= 2) {
                    oms.cancel_order(std::stoull(parts[1]));
                } else if (parts[0] == "SET_LATENCY" && parts.size() >= 2) {
                    exch.set_latency(std::stoull(parts[1]));
                    send_msg("EVT:LATENCY_SET|" + parts[1]);
                } else if (parts[0] == "KILL_SWITCH") {
                    risk.activate_kill_switch();
                    send_msg("EVT:KILL_SWITCH_ACTIVE");
                }
            }
            remnants = data;
        }
        std::cout << "Client disconnected.\n";
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
