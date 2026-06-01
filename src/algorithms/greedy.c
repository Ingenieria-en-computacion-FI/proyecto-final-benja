#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Algoritmos Greedy
 *
 * Un algoritmo greedy toma la mejor decisión LOCAL en cada paso
 * esperando llegar a un óptimo global.
 *
 * 1. First Fit Greedy  — primer hueco suficiente.  T: O(n)
 * 2. Best Fit          — hueco más ajustado.        T: O(n)
 * 3. Worst Fit         — hueco más grande.          T: O(n)
 * 4. SJF sort          — ordenar por burst_time.    T: O(n log n)
 *    Recurrencia merge sort: T(n)=2T(n/2)+O(n) → Θ(n log n)
 * --------------------------------------------------------------- */

/* Wrappers que delegan a las implementaciones en memory/ */
int mm_allocate_first_fit_greedy(MemoryManager* mm, int size) {
    return mm_allocate_first_fit(mm, size);
}

/* SJF Sort — merge sort sobre arrays paralelos (pids, burst_times) */
static void merge_sjf(int* pids, int* bt, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *lp = malloc(n1*sizeof(int)), *lb = malloc(n1*sizeof(int));
    int *rp = malloc(n2*sizeof(int)), *rb = malloc(n2*sizeof(int));

    for (int i = 0; i < n1; i++) { lp[i]=pids[l+i]; lb[i]=bt[l+i]; }
    for (int i = 0; i < n2; i++) { rp[i]=pids[m+1+i]; rb[i]=bt[m+1+i]; }

    int i=0, j=0, k=l;
    while (i<n1 && j<n2) {
        if (lb[i] <= rb[j]) { pids[k]=lp[i]; bt[k]=lb[i]; i++; }
        else                { pids[k]=rp[j]; bt[k]=rb[j]; j++; }
        k++;
    }
    while (i<n1) { pids[k]=lp[i]; bt[k]=lb[i]; i++; k++; }
    while (j<n2) { pids[k]=rp[j]; bt[k]=rb[j]; j++; k++; }

    free(lp); free(lb); free(rp); free(rb);
}

void sjf_sort(int* pids, int* burst_times, int n) {
    if (n <= 1) return;
    /* Divide y vencerás aplicado al ordenamiento */
    int mid = n / 2;
    sjf_sort(pids, burst_times, mid);
    sjf_sort(pids + mid, burst_times + mid, n - mid);
    merge_sjf(pids, burst_times, 0, mid - 1, n - 1);
}