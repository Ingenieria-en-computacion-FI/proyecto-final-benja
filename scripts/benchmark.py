"""
benchmark.py
Ejecuta ./bin/main con distintos tamaños de entrada y mide tiempos.
Exporta resultados a reports/csv/benchmark.csv.

Uso:
    python3 scripts/benchmark.py
"""
import subprocess
import time
import os
import csv

SIZES    = [10, 50, 100, 500, 1000]
BIN      = "./bin/main"
OUT_CSV  = "reports/csv/benchmark.csv"
OUT_DIR  = os.path.dirname(OUT_CSV)


def run_once(size: int) -> dict:
    os.makedirs("data/outputs", exist_ok=True)
    start = time.perf_counter()
    result = subprocess.run(
        [BIN, str(size)],
        capture_output=True,
        text=True,
    )
    elapsed = time.perf_counter() - start

    return {
        "size":        size,
        "time_ms":     round(elapsed * 1000, 4),
        "returncode":  result.returncode,
        "ok":          result.returncode == 0,
    }


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    rows = []

    print(f"{'Size':>8}  {'Time (ms)':>12}  {'OK':>4}")
    print("-" * 32)

    for size in SIZES:
        row = run_once(size)
        rows.append(row)
        print(f"{row['size']:>8}  {row['time_ms']:>12.4f}  "
              f"{'✓' if row['ok'] else '✗':>4}")

    with open(OUT_CSV, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)

    print(f"\n[benchmark] Resultados → {OUT_CSV}")


if __name__ == "__main__":
    main()