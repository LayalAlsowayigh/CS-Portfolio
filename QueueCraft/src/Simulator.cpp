#include "Simulator.hpp"
#include <fstream>
#include <iomanip>
#include <random>

int Simulator::poisson(std::mt19937& rng, double lambda) {
    std::poisson_distribution<> d(lambda);
    return d(rng);
}

void Simulator::enqueue_arrivals(int minute, std::mt19937& rng, int& next_id) {
    int e = poisson(rng, lambda_elderly);
    int f = poisson(rng, lambda_family);
    int r = poisson(rng, lambda_regular);
    for (int i=0; i<e; ++i) qE.emplace(next_id++, Group::Elderly, minute);
    for (int i=0; i<f; ++i) qF.emplace(next_id++, Group::Family,  minute);
    for (int i=0; i<r; ++i) qR.emplace(next_id++, Group::Regular, minute);
}

// Priority: Elderly > Family > Regular,
// but if we served many Regulars in a row (fairness_round), force E/F if waiting.
void Simulator::board_minute(int minute, int capacity, int& served_in_row,
                             std::vector<Passenger>& boarded) {
    auto take = [&](std::queue<Passenger>& q)->bool{
        if (q.empty()) return false;
        Passenger p = q.front(); q.pop();
        p.board_min = minute;
        boarded.push_back(p);
        return true;
    };

    for (int c=0; c<capacity; ++c) {
        bool served = false;

        // Fairness bump after runs of regulars
        if (served_in_row >= fairness_round) {
            if (take(qE)) { served = true; served_in_row = 0; }
            else if (take(qF)) { served = true; served_in_row = 0; }
            if (served) continue;
        }

        // Normal priority
        if (take(qE)) { served_in_row = 0; continue; }
        if (take(qF)) { served_in_row = 0; continue; }
        if (take(qR)) { served_in_row++; continue; }

        // No one waiting
        break;
    }
}

Metrics Simulator::compute_metrics(const std::vector<Passenger>& boarded, int total_minutes) const {
    auto avg = [](const std::vector<int>& v)->double {
        if (v.empty()) return 0.0;
        long long s = 0; for (int x : v) s += x;
        return static_cast<double>(s) / static_cast<double>(v.size());
    };

    std::vector<int> wE, wF, wR;
    int maxw = 0;
    for (const auto& p : boarded) {
        int w = wait_time(p);
        if (p.group == Group::Elderly)      wE.push_back(w);
        else if (p.group == Group::Family)  wF.push_back(w);
        else                                wR.push_back(w);
        if (w > maxw) maxw = w;
    }

    Metrics m;
    m.avg_wait_elderly = avg(wE);
    m.avg_wait_family  = avg(wF);
    m.avg_wait_regular = avg(wR);
    m.max_wait         = maxw;
    m.boarded_total    = static_cast<int>(boarded.size());
    m.throughput_per_min = (total_minutes > 0) ? double(m.boarded_total)/double(total_minutes) : 0.0;
    return m;
}

Metrics Simulator::run(const std::string& runname) {
    // reset queues & RNG
    qE = {}; qF = {}; qR = {};
    std::mt19937 rng(seed);
    int next_id = 1;
    int served_in_row = 0;
    std::vector<Passenger> boarded; boarded.reserve(minutes * capacity_per_min);

    for (int t=0; t<minutes; ++t) {
        enqueue_arrivals(t, rng, next_id);
        board_minute(t, capacity_per_min, served_in_row, boarded);
    }

    // CSV with per-passenger logs
    std::ofstream out("runs/" + runname + ".csv");
    out << "id,group,arrival_min,board_min,wait\n";
    for (const auto& p : boarded) {
        out << p.id << "," << group_to_str(p.group) << ","
            << p.arrival_min << "," << p.board_min << ","
            << wait_time(p) << "\n";
    }

    // One-line metrics CSV
    Metrics m = compute_metrics(boarded, minutes);
    std::ofstream mout("runs/" + runname + "_metrics.csv");
    mout << "avg_wait_elderly,avg_wait_family,avg_wait_regular,max_wait,boarded_total,throughput_per_min\n";
    mout << std::fixed
         << m.avg_wait_elderly << ","
         << m.avg_wait_family  << ","
         << m.avg_wait_regular << ","
         << m.max_wait << ","
         << m.boarded_total << ","
         << m.throughput_per_min << "\n";

    return m;
}
