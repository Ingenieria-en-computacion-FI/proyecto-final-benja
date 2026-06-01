"""
graphs.py
Genera gráficas de benchmarking y análisis de complejidad.

Uso:
    python3 scripts/graphs.py
"""
import os
import csv
import math
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

OUT_DIR = "reports/png"
CSV_DIR = "reports/csv"

os.makedirs(OUT_DIR, exist_ok=True)


def load_csv(path: str) -> list[dict]:
    if not os.path.exists(path):
        return []
    with open(path) as f:
        return list(csv.DictReader(f))


# ── Gráfica 1: Tiempo de ejecución vs tamaño de entrada ──────────
def plot_benchmark():
    rows = load_csv(f"{CSV_DIR}/benchmark.csv")
    if not rows:
        print("[graphs] Sin datos de benchmark, saltando...")
        return

    sizes = [int(r["size"])    for r in rows]
    times = [float(r["time_ms"]) for r in rows]

    fig, ax = plt.subplots(figsize=(8, 4))
    ax.plot(sizes, times, "o-", color="#2563EB", linewidth=2,
            markersize=6, label="Tiempo real")

    # Curva O(n log n) de referencia
    if sizes:
        ref = [s * math.log2(max(s, 2)) for s in sizes]
        scale = times[-1] / ref[-1] if ref[-1] else 1
        ax.plot(sizes, [r * scale for r in ref],
                "--", color="#94A3B8", linewidth=1.2, label="O(n log n) ref.")

    ax.set_xlabel("Número de procesos")
    ax.set_ylabel("Tiempo (ms)")
    ax.set_title("Tiempo de ejecución vs tamaño de entrada")
    ax.legend()
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    path = f"{OUT_DIR}/benchmark_time.png"
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[graphs] → {path}")


# ── Gráfica 2: Comparativa de schedulers (simulada) ──────────────
def plot_scheduler_comparison():
    n = np.arange(1, 21)  # 1..20 procesos
    quantum = 3

    # Tiempo de espera promedio teórico por scheduler
    fifo_wait = n * (n - 1) / 2             # O(n²) sin ordenar
    sjf_wait  = n * (n - 1) / 4             # mitad que FIFO (óptimo)
    rr_wait   = n * (n - 1) / 2 * quantum   # proporcional al quantum

    fig, ax = plt.subplots(figsize=(8, 4))
    ax.plot(n, fifo_wait, "o-",  color="#EF4444", label="FIFO",         linewidth=2)
    ax.plot(n, sjf_wait,  "s-",  color="#22C55E", label="SJF",          linewidth=2)
    ax.plot(n, rr_wait,   "^--", color="#F59E0B", label=f"RR (q={quantum})", linewidth=2)

    ax.set_xlabel("Número de procesos (n)")
    ax.set_ylabel("Tiempo de espera promedio (unidades)")
    ax.set_title("Comparativa de schedulers — tiempo de espera teórico")
    ax.legend()
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    path = f"{OUT_DIR}/scheduler_comparison.png"
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[graphs] → {path}")


# ── Gráfica 3: Complejidad de algoritmos de memoria ──────────────
def plot_memory_complexity():
    n = np.arange(1, 101)

    fig, ax = plt.subplots(figsize=(8, 4))
    ax.plot(n, n,             label="First/Best/Worst Fit O(n)",  color="#2563EB")
    ax.plot(n, n * np.log2(n), label="Compactación O(n log n)",    color="#7C3AED")
    ax.plot(n, n**2,           label="Backtracking O(n²)",         color="#DC2626", linestyle="--")

    ax.set_xlabel("Número de bloques (n)")
    ax.set_ylabel("Operaciones (relativo)")
    ax.set_title("Complejidad de algoritmos de memoria")
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_ylim(0, 500)
    fig.tight_layout()
    path = f"{OUT_DIR}/memory_complexity.png"
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[graphs] → {path}")


if __name__ == "__main__":
    plot_benchmark()
    plot_scheduler_comparison()
    plot_memory_complexity()
    print("[graphs] Todas las gráficas generadas.")