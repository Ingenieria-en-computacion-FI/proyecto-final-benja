#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Programación Dinámica Incremental
 *
 * Variante donde los procesos llegan uno a uno en tiempo real.
 * En lugar de recalcular toda la tabla cada vez, se actualiza
 * solo la fila afectada por el nuevo proceso.
 *
 * Estado: dp[w] = máximo valor alcanzable con capacidad w.
 * Actualización al agregar proceso (weight, value):
 *   Para w de W hacia weight (recorrido inverso — evita usar el
 *   mismo proceso dos veces, propiedad del 0/1 knapsack):
 *     dp[w] = max(dp[w], value + dp[w - weight])
 *
 * Complejidad temporal por actualización: O(W)
 * Complejidad espacial: O(W) — solo un arreglo 1D
 *
 * Ventaja: no se recalcula la tabla completa al llegar un proceso.
 * --------------------------------------------------------------- */

int dp_knapsack_incremental(int* dp, int W, int new_weight, int new_value) {
    /* Recorrido inverso para garantizar 0/1 (no repetición) */
    for (int w = W; w >= new_weight; w--) {
        int candidate = new_value + dp[w - new_weight];
        if (candidate > dp[w]) dp[w] = candidate;
    }
    return dp[W]; /* valor óptimo actual con capacidad W */
}