#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* ---------------------------------------------------------------
 * Coalescencia de bloques libres adyacentes.
 *
 * Se llama automáticamente desde mm_free, pero también puede
 * invocarse manualmente tras múltiples liberaciones.
 *
 * Ejemplo:
 *   Antes:  [P1:200][300 libres][100 libres][P2:150][250 libres]
 *   Después:[P1:200][400 libres][P2:150][250 libres]
 *
 * Algoritmo:
 *   Recorre la lista de izquierda a derecha (usando ->next).
 *   Si cur y cur->next están ambos libres, los fusiona en cur.
 *   No avanza cuando fusiona (puede haber un tercero adyacente).
 *
 * Complejidad temporal: O(n) — un recorrido lineal.
 * Complejidad espacial: O(1) — in-place, sin estructuras auxiliares.
 *
 * Recurrencia (si se aplica recursivamente, no es el caso aquí):
 *   T(n) = T(n-1) + O(1) → T(n) = O(n)
 * --------------------------------------------------------------- */

void mm_coalesce_explicit(MemoryManager* mm) {
    MemoryBlock* cur = mm->head;
    int merged = 0;

    while (cur != NULL && cur->next != NULL) {
        if (cur->free && cur->next->free) {
            MemoryBlock* victim = cur->next;
            cur->size += victim->size;
            cur->next  = victim->next;
            if (victim->next) victim->next->prev = cur;
            free(victim);
            merged++;
            /* no avanzar: revisar si el siguiente también está libre */
        } else {
            cur = cur->next;
        }
    }

    if (merged > 0)
        printf("[coalescence] %d bloque(s) fusionado(s)\n", merged);
}