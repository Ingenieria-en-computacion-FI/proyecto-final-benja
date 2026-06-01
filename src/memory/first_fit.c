#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* ---------------------------------------------------------------
 * Administrador de Memoria — lista doblemente ligada de MemoryBlock.
 *
 * mm_create         → O(1)
 * mm_allocate_first_fit → O(n)  fuerza bruta: recorre hasta encontrar hueco
 * mm_free           → O(n)  busca el bloque por pid
 * mm_coalesce       → O(n)  un recorrido de la lista
 * mm_destroy        → O(n)
 * --------------------------------------------------------------- */

MemoryManager* mm_create(int total_memory) {
    MemoryManager* mm = malloc(sizeof(MemoryManager));
    if (!mm) { perror("mm_create"); exit(EXIT_FAILURE); }

    /* Bloque inicial: toda la memoria libre */
    MemoryBlock* block = malloc(sizeof(MemoryBlock));
    if (!block) { perror("mm_create block"); exit(EXIT_FAILURE); }
    block->start = 0;
    block->size  = total_memory;
    block->free  = 1;
    block->pid   = -1;
    block->next  = NULL;
    block->prev  = NULL;

    mm->head         = block;
    mm->total_memory = total_memory;
    return mm;
}

/* First Fit (greedy): primer bloque libre con tamaño suficiente.
 * Devuelve el pid asignado (== pid pasado) o -1 si no hay espacio.
 * El pid se usa como identificador para mm_free. */
int mm_allocate_first_fit(MemoryManager* mm, int size) {
    static int next_pid = 1;       /* pid autoincremental si se llama sin pid externo */
    return mm_allocate_first_fit_pid(mm, size, next_pid++);
}

int mm_allocate_first_fit_pid(MemoryManager* mm, int size, int pid) {
    MemoryBlock* cur = mm->head;

    while (cur != NULL) {
        if (cur->free && cur->size >= size) {
            /* Fragmentar si sobra espacio */
            if (cur->size > size) {
                MemoryBlock* remainder = malloc(sizeof(MemoryBlock));
                if (!remainder) { perror("mm_allocate"); exit(EXIT_FAILURE); }
                remainder->start = cur->start + size;
                remainder->size  = cur->size  - size;
                remainder->free  = 1;
                remainder->pid   = -1;
                remainder->next  = cur->next;
                remainder->prev  = cur;
                if (cur->next) cur->next->prev = remainder;
                cur->next = remainder;
            }
            cur->size = size;
            cur->free = 0;
            cur->pid  = pid;
            return pid;
        }
        cur = cur->next;
    }
    return -1; /* no hay hueco */
}

void mm_free(MemoryManager* mm, int pid) {
    MemoryBlock* cur = mm->head;
    while (cur != NULL) {
        if (!cur->free && cur->pid == pid) {
            cur->free = 1;
            cur->pid  = -1;
            mm_coalesce(mm);  /* combinar bloques adyacentes inmediatamente */
            return;
        }
        cur = cur->next;
    }
    fprintf(stderr, "mm_free: pid %d no encontrado\n", pid);
}

/* Coalescencia: un recorrido que combina bloques libres contiguos.
 * Usa navegación hacia atrás (prev) para fusionar con el bloque anterior.
 *
 * Ejemplo antes:  [P1:200][300 libres][100 libres][P2:150]
 * Ejemplo después: [P1:200][400 libres][P2:150]
 */
void mm_coalesce(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    while (cur != NULL && cur->next != NULL) {
        if (cur->free && cur->next->free) {
            /* Fusionar cur con cur->next */
            MemoryBlock* to_free = cur->next;
            cur->size += to_free->size;
            cur->next  = to_free->next;
            if (to_free->next) to_free->next->prev = cur;
            free(to_free);
            /* No avanzar: puede haber otro bloque libre contiguo */
        } else {
            cur = cur->next;
        }
    }
}

void mm_print(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    printf("[Memoria | total=%d]:\n", mm->total_memory);
    while (cur != NULL) {
        printf("  [start=%d size=%d %s pid=%d]\n",
               cur->start, cur->size,
               cur->free ? "LIBRE" : "OCUP.",
               cur->pid);
        cur = cur->next;
    }
}

void mm_destroy(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    while (cur != NULL) {
        MemoryBlock* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(mm);
}