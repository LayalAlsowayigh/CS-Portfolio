# QueueCraft — Transit Boarding Queue Simulator (C++17)

QueueCraft simulates boarding at a transit stop using **priority queues** (elderly, families, regular) with a fairness rule to prevent starvation. It writes per-passenger logs and summary metrics to CSV. A tiny Python script produces a chart of average wait times by group.

## Features
- Poisson arrivals (seeded RNG for reproducibility)
- Priority + fairness policy (configurable)
- CSV outputs: `runs/run1.csv` and `runs/run1_metrics.csv`
- Optional Python plot: `runs/avg_wait.png`

## Build & Run (macOS / Linux)
```bash
g++ -std=c++17 -Wall -Wextra src/*.cpp -o queuecraft
./queuecraft
python3 analyze.py runs/run1.csv   # optional chart
