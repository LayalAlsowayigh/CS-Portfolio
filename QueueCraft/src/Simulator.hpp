#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Passenger.hpp"
#include <queue>
#include <random>
#include <string>
#include <vector>

struct Metrics {
    double avg_wait_elderly = 0.0;
    double avg_wait_family  = 0.0;
    double avg_wait_regular = 0.0;
    int    max_wait         = 0;
    int    boarded_total    = 0;
    double throughput_per_min = 0.0;
};

class Simulator {
public:
    // --- Config (tweakable) ---
    int minutes = 30;             // total simulation length
    int capacity_per_min = 6;     // how many can board per minute
    double lambda_elderly = 0.8;  // poisson mean arrivals/min
    double lambda_family  = 1.0;
    double lambda_regular = 2.0;
    int fairness_round    = 3;    // after N regulars in a row, prioritize E/F
    unsigned seed         = 42;   // RNG seed for reproducibility

    // Run simulation, write CSVs under runs/, and return metrics.
    Metrics run(const std::string& runname);

private:
    std::queue<Passenger> qE, qF, qR;

    int  poisson(std::mt19937& rng, double lambda);
    void enqueue_arrivals(int minute, std::mt19937& rng, int& next_id);
    void board_minute(int minute, int capacity, int& served_regulars_in_row,
                      std::vector<Passenger>& boarded);
    Metrics compute_metrics(const std::vector<Passenger>& boarded, int total_minutes) const;

    static int wait_time(const Passenger& p) {
        return (p.board_min < 0) ? 0 : (p.board_min - p.arrival_min);
    }
};

#endif
