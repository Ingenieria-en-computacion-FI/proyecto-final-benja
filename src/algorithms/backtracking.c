#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Backtracking — asignación de memoria con recuperación de estados.
 *
 * Problema: dado un conjunto de N procesos con tamaños sizes[],
 * intentar asignar memoria a todos usando mm_allocate_first_fit.
 * Si alguno falla, deshacer todas las asignaciones anteriores y
 * reportar que no hay asignación válida completa.
 *
 * La pila de PIDs asignados permite el "backtrack" (deshacer).
 *
 * Complejidad temporal: O(n) caso promedio, O(2^n) peor caso
 *   (si hay muchas combinaciones de asignaciones posibles).
 * Complejidad espacial: O(n) — pila de recursión + array de PIDs.
 *
 * Recurrencia: T(n) = T(n-1) + O(n)  → T(n) = O(n²) promedio.
 * --------------------------------------------------------------- */

int backtrack_allocate(MemoryManager* mm, int* sizes, int* pids,
                       int n, int idx) {
    /* Caso base: todos asignados */
    if (idx == n) return 1;

    int pid = mm_allocate_first_fit_pid(mm, sizes[idx], pids[idx]);
    if (pid == -1) {
        printf("[backtracking] Fallo en idx=%d (size=%d) — retrocediendo\n",
               idx, sizes[idx]);
        return 0;  /* no hay hueco → backtrack al nivel anterior */
    }

    printf("[backtracking] Asignado PID=%d size=%d en idx=%d\n",
           pid, sizes[idx], idx);

    /* Intentar asignar el siguiente proceso */
    if (backtrack_allocate(mm, sizes, pids, n, idx + 1))
        return 1;

    /* Si el siguiente falló, liberar este y retroceder */
    printf("[backtracking] Deshaciendo PID=%d\n", pid);
    mm_free(mm, pid);
    return 0;
}