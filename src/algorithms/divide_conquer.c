#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Divide y Vencerás — merge sort de bloques de memoria.
 *
 * Se usa en la compactación (ver compactation.c) para reordenar
 * los bloques ocupados por dirección start.
 *
 * Recurrencia: T(n) = 2T(n/2) + O(n)
 * Por el Teorema Maestro: caso 2, a=2, b=2, f(n)=O(n)
 *   → n^(log_b a) = n^1 = n = f(n)  → T(n) = Θ(n log n)
 *
 * Complejidad temporal: Θ(n log n)
 * Complejidad espacial: O(n) — arreglos temporales L y R
 * --------------------------------------------------------------- */

static void merge_blocks(MemoryBlock** arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    MemoryBlock** L = malloc(n1 * sizeof(MemoryBlock*));
    MemoryBlock** R = malloc(n2 * sizeof(MemoryBlock*));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i]->start <= R[j]->start) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

void merge_sort_blocks(MemoryBlock** arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_blocks(arr, left, mid);      /* Divide: mitad izquierda */
    merge_sort_blocks(arr, mid + 1, right); /* Divide: mitad derecha   */
    merge_blocks(arr, left, mid, right);    /* Vencerás: combinar       */
}

void divide_conquer_compact(MemoryManager* mm) {
    /* Delegar a la implementación completa en compactation.c */
    extern void mm_compact(MemoryManager*);
    mm_compact(mm);
}