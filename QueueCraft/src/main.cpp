/**
 * Author: Layal Alsowayigh
 * Project: QueueCraft — Transit Boarding Queue Simulator
 * Description: This program simulates passenger boarding for public transit
 * Date: August 2025
 */


#include "Simulator.hpp"
#include <filesystem>
#include <iostream>

int main() {
    std::cout << "QueueCraft — Transit Boarding Queue Simulator (C++17)\n";
    std::filesystem::create_directories("runs");

    Simulator sim;
    sim.minutes          = 30;
    sim.capacity_per_min = 4;
    sim.lambda_elderly   = 0.9;
    sim.lambda_family    = 1.2;
    sim.lambda_regular   = 2.6;
    sim.fairness_round   = 3;
    sim.seed             = 42;


    auto m = sim.run("run1");

    std::cout << "=== Metrics (run1) ===\n";
    std::cout << "avg wait (elderly): " << m.avg_wait_elderly << " min\n";
    std::cout << "avg wait (family) : " << m.avg_wait_family  << " min\n";
    std::cout << "avg wait (regular): " << m.avg_wait_regular << " min\n";
    std::cout << "max wait          : " << m.max_wait << " min\n";
    std::cout << "boarded total     : " << m.boarded_total << "\n";
    std::cout << "throughput/min    : " << m.throughput_per_min << "\n";
    std::cout << "CSV written: runs/run1.csv and runs/run1_metrics.csv\n";
    return 0;
}
