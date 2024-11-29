#include "crow.h"
#include "json/single_include/nlohmann/json.hpp"
#include <asio.hpp>
#include <vector>
#include <algorithm>
#include <queue>

using json = nlohmann::json;

struct Process {
    int id;
    int arrival;
    int burst;
    int priority;
    int waiting;
    int turnaround;
};

int calculateFCFS(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    int currentTime = 0;
    for (auto& p : processes) {
        p.waiting = std::max(0, currentTime - p.arrival);
        p.turnaround = p.waiting + p.burst;
        currentTime += p.burst;
    }
    return currentTime;
}

void calculateSJF(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.burst < b.burst;
    });

    int currentTime = 0;
    for (auto& p : processes) {
        p.waiting = std::max(0, currentTime - p.arrival);
        p.turnaround = p.waiting + p.burst;
        currentTime += p.burst;
    }
}

void calculatePriority(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return (a.arrival == b.arrival) ? (a.priority < b.priority) : (a.arrival < b.arrival);
    });

    int currentTime = 0;
    for (auto& p : processes) {
        p.waiting = std::max(0, currentTime - p.arrival);
        p.turnaround = p.waiting + p.burst;
        currentTime += p.burst;
    }
}

void calculateRoundRobin(std::vector<Process>& processes, int quantum) {
    std::queue<Process> readyQueue;
    int currentTime = 0;
    int remainingTime[processes.size()];

    for (size_t i = 0; i < processes.size(); ++i) {
        readyQueue.push(processes[i]);
        remainingTime[i] = processes[i].burst;
    }

    while (!readyQueue.empty()) {
        Process currentProcess = readyQueue.front();
        readyQueue.pop();

        int executeTime = std::min(quantum, remainingTime[currentProcess.id - 1]);
        remainingTime[currentProcess.id - 1] -= executeTime;
        currentTime += executeTime;

        if (remainingTime[currentProcess.id - 1] > 0) {
            readyQueue.push(currentProcess);
        } else {
            currentProcess.waiting = currentTime - currentProcess.arrival - currentProcess.burst;
            currentProcess.turnaround = currentProcess.waiting + currentProcess.burst;
        }
    }
}

int main() {
    crow::SimpleApp app;
    CROW_ROUTE(app, "/fcfs").methods("POST"_method)([](const crow::request& req) {
        auto req_data = json::parse(req.body);
        std::vector<Process> processes;

        for (const auto& item : req_data["processes"]) {
            processes.push_back({ item["id"], item["arrival"], item["burst"], item["priority"], 0, 0 });
        }

        int ans = calculateFCFS(processes);

        json result;
        for (const auto& p : processes) {
            result["processes"].push_back({ 
                {"id", p.id},
                {"arrival", p.arrival},
                {"burst", p.burst},
                {"priority", p.priority},
                {"waiting", p.waiting},
                {"turnaround", p.turnaround}
            });
        }

        return crow::response(result.dump());
    });


    // CROW_ROUTE(app, "/fcfs").methods("POST"_method)([](const crow::request& req) {
    //     auto req_data = json::parse(req.body);
    //     std::vector<Process> processes;

    //     for (const auto& item : req_data["processes"]) {
    //         processes.push_back({ item["id"], item["arrival"], item["burst"], item["priority"], 0, 0 });
    //     }

    //     int ans = calculateFCFS(processes);

    //     json result;
    //     for (const auto& p : processes) {
    //         result["processes"].push_back({ 
    //             {"id", p.id},
    //             {"arrival", p.arrival},
    //             {"burst", p.burst},
    //             {"priority", p.priority},
    //             {"waiting", p.waiting},
    //             {"turnaround", p.turnaround}
    //         });
    //     }

    //     return crow::response(result.dump());
    // });

    CROW_ROUTE(app, "/sjf").methods("POST"_method)([](const crow::request& req) {
        auto req_data = json::parse(req.body);
        std::vector<Process> processes;

        for (const auto& item : req_data["processes"]) {
            processes.push_back({ item["id"], item["arrival"], item["burst"], item["priority"], 0, 0 });
        }

        calculateSJF(processes);

        json result;
        for (const auto& p : processes) {
            result["processes"].push_back({ 
                {"id", p.id},
                {"arrival", p.arrival},
                {"burst", p.burst},
                {"priority", p.priority},
                {"waiting", p.waiting},
                {"turnaround", p.turnaround}
            });
        }

        return crow::response(result.dump());
    });

    CROW_ROUTE(app, "/priority").methods("POST"_method)([](const crow::request& req) {
        auto req_data = json::parse(req.body);
        std::vector<Process> processes;

        for (const auto& item : req_data["processes"]) {
            processes.push_back({ item["id"], item["arrival"], item["burst"], item["priority"], 0, 0 });
        }

        calculatePriority(processes);

        json result;
        for (const auto& p : processes) {
            result["processes"].push_back({ 
                {"id", p.id},
                {"arrival", p.arrival},
                {"burst", p.burst},
                {"priority", p.priority},
                {"waiting", p.waiting},
                {"turnaround", p.turnaround}
            });
        }

        return crow::response(result.dump());
    });

    CROW_ROUTE(app, "/roundrobin").methods("POST"_method)([](const crow::request& req) {
        auto req_data = json::parse(req.body);
        std::vector<Process> processes;
        int quantum = req_data["quantum"];

        for (const auto& item : req_data["processes"]) {
            processes.push_back({ item["id"], item["arrival"], item["burst"], item["priority"], 0, 0 });
        }

        calculateRoundRobin(processes, quantum);

        json result;
        for (const auto& p : processes) {
            result["processes"].push_back({ 
                {"id", p.id},
                {"arrival", p.arrival},
                {"burst", p.burst},
                {"priority", p.priority},
                {"waiting", p.waiting},
                {"turnaround", p.turnaround}
            });
        }

        return crow::response(result.dump());
    });

    app.port(8080).multithreaded().run();
    return 0;
}