#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Fuerza Bruta — búsqueda exhaustiva de huecos de memoria.
 *
 * Recorre TODOS los bloques de la lista sin heurística,
 * devolviendo el primero que cumple la condición de tamaño.
 *
 * Complejidad temporal: O(n) — recorrido lineal completo.
 * Complejidad espacial: O(1) — no usa estructuras auxiliares.
 *
 * Diferencia con First Fit greedy: aquí se recorre toda la lista
 * incluso si ya se encontró un candidato (para poder registrar
 * estadísticas de cuántos bloques se exploraron).
 * --------------------------------------------------------------- */

MemoryBlock* brute_force_find_block(MemoryManager* mm, int size) {
    MemoryBlock* result    = NULL;
    MemoryBlock* cur       = mm->head;
    int          explored  = 0;

    while (cur != NULL) {
        explored++;
        if (cur->free && cur->size >= size) {
            if (result == NULL) result = cur;  /* primer candidato */
        }
        cur = cur->next;
    }

    printf("[brute_force] bloques explorados: %d, candidato encontrado: %s\n",
           explored, result ? "sí" : "no");
    return result;
}