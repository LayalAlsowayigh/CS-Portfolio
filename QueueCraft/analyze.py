# Quick analysis/plot for QueueCraft
# Usage: python3 analyze.py runs/run1.csv
import sys, csv
from collections import defaultdict
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("usage: python3 analyze.py runs/run1.csv")
    raise SystemExit(1)

path = sys.argv[1]
waits = defaultdict(list)

with open(path, newline='') as f:
    r = csv.DictReader(f)
    for row in r:
        waits[row['group']].append(int(row['wait']))

avg = {g: (sum(v)/len(v) if v else 0.0) for g, v in waits.items()}
print("Average waits (min):", avg)

groups = ['elderly', 'family', 'regular']
values = [avg.get(g, 0.0) for g in groups]

plt.figure()
plt.title('Average Wait by Group')
plt.bar(groups, values)
plt.xlabel('Group')
plt.ylabel('Avg Wait (min)')
plt.tight_layout()
plt.savefig('runs/avg_wait.png')
print("Saved chart: runs/avg_wait.png")
