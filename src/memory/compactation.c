#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* ---------------------------------------------------------------
 * Compactación de memoria — Divide y Vencerás.
 *
 * Problema: la memoria libre está fragmentada en N bloques pequeños.
 * Solución: mover todos los bloques OCUPADOS al inicio, dejando
 *           un único bloque libre al final.
 *
 * Estrategia Divide y Vencerás:
 *   1. Contar bloques → O(n)
 *   2. Copiar bloques ocupados a arreglo auxiliar → O(n)
 *   3. Merge sort del arreglo por start (orden físico) → O(n log n)
 *      Recurrencia: T(n) = 2T(n/2) + O(n) → Θ(n log n) por Master Theorem
 *   4. Reasignar direcciones start de forma contigua → O(n)
 *   5. Reconstruir lista ligada + bloque libre final → O(n)
 *
 * Complejidad temporal total: O(n log n)
 * Complejidad espacial: O(n) — arreglo auxiliar de punteros
 * --------------------------------------------------------------- */

static void merge(MemoryBlock** arr, int l, int m, int r) {
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

static void merge_sort_blocks_compact(MemoryBlock** arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_blocks_compact(arr, left, mid);
    merge_sort_blocks_compact(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mm_compact(MemoryManager* mm) {
    /* 1. Contar bloques ocupados */
    int count = 0;
    MemoryBlock* cur = mm->head;
    while (cur) { if (!cur->free) count++; cur = cur->next; }

    if (count == 0) return;

    /* 2. Recopilar punteros a bloques ocupados */
    MemoryBlock** occupied = malloc(count * sizeof(MemoryBlock*));
    if (!occupied) { perror("mm_compact"); exit(EXIT_FAILURE); }
    int idx = 0;
    cur = mm->head;
    while (cur) { if (!cur->free) occupied[idx++] = cur; cur = cur->next; }

    /* 3. Ordenar por dirección start (divide y vencerás) */
    merge_sort_blocks_compact(occupied, 0, count - 1);

    /* 4. Reasignar starts de forma contigua desde 0 */
    int next_start = 0;
    for (int i = 0; i < count; i++) {
        occupied[i]->start = next_start;
        next_start += occupied[i]->size;
    }

    /* 5. Reconectar lista: ocupados primero, un bloque libre al final */
    for (int i = 0; i < count - 1; i++) {
        occupied[i]->next = occupied[i + 1];
        occupied[i + 1]->prev = occupied[i];
    }
    occupied[0]->prev = NULL;
    mm->head = occupied[0];

    int free_space = mm->total_memory - next_start;
    if (free_space > 0) {
        MemoryBlock* free_block = malloc(sizeof(MemoryBlock));
        if (!free_block) { perror("mm_compact free_block"); exit(EXIT_FAILURE); }
        free_block->start = next_start;
        free_block->size  = free_space;
        free_block->free  = 1;
        free_block->pid   = -1;
        free_block->next  = NULL;
        free_block->prev  = occupied[count - 1];
        occupied[count - 1]->next = free_block;
    } else {
        occupied[count - 1]->next = NULL;
    }

    /* Liberar viejos bloques libres que ya no están en la lista */
    cur = mm->head;
    while (cur) {
        MemoryBlock* nxt = cur->next;
        /* los que no son occupied[] y son free → ya se desvincularon arriba */
        cur = nxt;
    }

    free(occupied);
    printf("[compactación] Memoria compactada. Libre contigua: %d\n", free_space);
}