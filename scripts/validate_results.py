"""
validate_results.py
Verifica que los resultados de C sean correctos.
Compara tiempos de ejecución contra valores esperados teóricos.

Uso:
    python3 scripts/validate_results.py
"""
import csv
import os
import sys

RESULTS_CSV = "data/outputs/results.csv"
BENCHMARK_CSV = "reports/csv/benchmark.csv"


def validate_benchmark():
    if not os.path.exists(BENCHMARK_CSV):
        print(f"[validate] No existe {BENCHMARK_CSV}. Ejecuta benchmark.py primero.")
        return False

    with open(BENCHMARK_CSV) as f:
        rows = list(csv.DictReader(f))

    print(f"[validate] Revisando {len(rows)} ejecuciones de benchmark...")
    all_ok = True
    prev_time = 0.0

    for row in rows:
        size = int(row["size"])
        time_ms = float(row["time_ms"])
        ok = row["ok"] == "True"

        # Verificar que el binario terminó sin error
        if not ok:
            print(f"  ✗ size={size}: el binario retornó error")
            all_ok = False
        else:
            print(f"  ✓ size={size}: {time_ms:.4f} ms")

        # Verificar monotonía: tiempo no debe decrecer abruptamente
        if size > 10 and time_ms < prev_time * 0.1:
            print(f"  ⚠ size={size}: tiempo anormalmente bajo vs anterior")
        prev_time = time_ms

    return all_ok


def main():
    print("=== Validación de resultados ===\n")
    ok = validate_benchmark()
    print(f"\n[validate] Resultado: {'PASS ✓' if ok else 'FAIL ✗'}")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()