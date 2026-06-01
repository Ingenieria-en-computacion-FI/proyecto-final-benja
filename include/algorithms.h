#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memory_manager.h"

/* ---------------------------------------------------------------
 * Algoritmos de asignación de memoria y optimización.
 * Complejidades documentadas por función.
 * --------------------------------------------------------------- */

/* --- Fuerza bruta -------------------------------------------- */
/* Búsqueda exhaustiva del primer bloque que cumple el tamaño.
 * T: O(n), E: O(1) */
MemoryBlock* brute_force_find_block(MemoryManager* mm, int size);

/* --- Greedy ---------------------------------------------------- */
/* First Fit: primer bloque libre suficiente. T: O(n), E: O(1) */
int mm_allocate_first_fit_greedy(MemoryManager* mm, int size);

/* Best Fit: bloque libre más pequeño que alcanza. T: O(n), E: O(1) */
int mm_allocate_best_fit(MemoryManager* mm, int size, int pid);

/* Worst Fit: bloque libre más grande disponible. T: O(n), E: O(1) */
int mm_allocate_worst_fit(MemoryManager* mm, int size, int pid);

/* Shortest Job First: ordena procesos por burst_time. T: O(n log n), E: O(n) */
void sjf_sort(int* pids, int* burst_times, int n);

/* --- Backtracking --------------------------------------------- */
/* Busca una asignación válida para un conjunto de procesos.
 * T: O(2^n) peor caso, E: O(n) pila de recursión */
int backtrack_allocate(MemoryManager* mm, int* sizes, int* pids, int n, int idx);

/* --- Divide y Vencerás --------------------------------------- */
/* Compactación: mueve todos los bloques libres al final.
 * T: O(n log n), E: O(n) */
void divide_conquer_compact(MemoryManager* mm);

/* Merge sort auxiliar sobre arreglo de bloques por dirección start.
 * T: O(n log n), E: O(n) — recurrencia T(n)=2T(n/2)+O(n) → Θ(n log n) */
void merge_sort_blocks(MemoryBlock** arr, int left, int right);

/* --- Programación dinámica (opcional) ------------------------ */
/* Knapsack: maximiza procesos que caben en memoria disponible.
 * Ascendente (bottom-up). T: O(n*W), E: O(n*W) */
int dp_knapsack_bottomup(int* weights, int* values, int n, int W);

/* Descendente (top-down con memoización). T: O(n*W), E: O(n*W) */
int dp_knapsack_topdown(int* weights, int* values, int n, int W, int** memo);

/* Incremental: recalcula solo la fila afectada al agregar un proceso.
 * T: O(W) por actualización, E: O(W) */
int dp_knapsack_incremental(int* dp, int W, int new_weight, int new_value);

#endif