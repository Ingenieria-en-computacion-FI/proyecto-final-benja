"""
generate_processes.py
Genera procesos aleatorios y los exporta a CSV para que C los lea.

Uso:
    python3 scripts/generate_processes.py [--n 20] [--out data/inputs/processes.csv]
"""
import argparse
import random
import csv
import os

def generate_processes(n: int, seed: int = 42) -> list[dict]:
    random.seed(seed)
    processes = []
    for i in range(1, n + 1):
        processes.append({
            "pid":             i,
            "burst_time":      random.randint(1, 20),
            "priority":        random.randint(1, 5),
            "memory_required": random.randint(10, 512),
        })
    return processes

def export_csv(processes: list[dict], path: str) -> None:
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=processes[0].keys())
        writer.writeheader()
        writer.writerows(processes)
    print(f"[generate] {len(processes)} procesos → {path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--n",   type=int, default=10, help="Número de procesos")
    parser.add_argument("--out", type=str, default="data/inputs/processes.csv")
    args = parser.parse_args()

    procs = generate_processes(args.n)
    export_csv(procs, args.out)

    # Vista previa
    for p in procs:
        print(f"  PID={p['pid']:3d}  burst={p['burst_time']:3d}  "
              f"mem={p['memory_required']:4d}  prio={p['priority']}")