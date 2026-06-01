#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* ---------------------------------------------------------------
 * Best Fit y Worst Fit — estrategias greedy de asignación.
 *
 * Best Fit:  elige el bloque libre MÁS PEQUEÑO que alcanza.
 *   Pro:  desperdicia menos memoria interna.
 *   Con:  genera muchos bloques pequeños inutilizables.
 *   T: O(n), E: O(1)
 *
 * Worst Fit: elige el bloque libre MÁS GRANDE disponible.
 *   Pro:  deja restos más grandes (más útiles).
 *   Con:  puede agotar bloques grandes rápidamente.
 *   T: O(n), E: O(1)
 * --------------------------------------------------------------- */

int mm_allocate_best_fit(MemoryManager* mm, int size, int pid) {
    MemoryBlock* best = NULL;
    MemoryBlock* cur  = mm->head;

    /* Un recorrido completo para encontrar el bloque más ajustado */
    while (cur != NULL) {
        if (cur->free && cur->size >= size) {
            if (best == NULL || cur->size < best->size)
                best = cur;
        }
        cur = cur->next;
    }

    if (best == NULL) return -1;

    /* Fragmentar si sobra espacio */
    if (best->size > size) {
        MemoryBlock* remainder = malloc(sizeof(MemoryBlock));
        if (!remainder) { perror("best_fit"); exit(EXIT_FAILURE); }
        remainder->start = best->start + size;
        remainder->size  = best->size  - size;
        remainder->free  = 1;
        remainder->pid   = -1;
        remainder->next  = best->next;
        remainder->prev  = best;
        if (best->next) best->next->prev = remainder;
        best->next = remainder;
    }

    best->size = size;
    best->free = 0;
    best->pid  = pid;
    return pid;
}

int mm_allocate_worst_fit(MemoryManager* mm, int size, int pid) {
    MemoryBlock* worst = NULL;
    MemoryBlock* cur   = mm->head;

    /* Un recorrido completo para encontrar el bloque más grande */
    while (cur != NULL) {
        if (cur->free && cur->size >= size) {
            if (worst == NULL || cur->size > worst->size)
                worst = cur;
        }
        cur = cur->next;
    }

    if (worst == NULL) return -1;

    if (worst->size > size) {
        MemoryBlock* remainder = malloc(sizeof(MemoryBlock));
        if (!remainder) { perror("worst_fit"); exit(EXIT_FAILURE); }
        remainder->start = worst->start + size;
        remainder->size  = worst->size  - size;
        remainder->free  = 1;
        remainder->pid   = -1;
        remainder->next  = worst->next;
        remainder->prev  = worst;
        if (worst->next) worst->next->prev = remainder;
        worst->next = remainder;
    }

    worst->size = size;
    worst->free = 0;
    worst->pid  = pid;
    return pid;
}