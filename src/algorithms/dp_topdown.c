#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/* ---------------------------------------------------------------
 * Programación Dinámica — Knapsack 0/1 (Top-Down / Descendente)
 *
 * Misma recurrencia que bottom-up, pero con memoización:
 * se calcula dp[i][w] solo si se necesita, guardando el resultado.
 *
 * dp[i][w] = -1  → no calculado aún
 * dp[i][w] >= 0  → resultado cacheado
 *
 * Complejidad temporal: O(n * W) — cada celda se calcula una vez.
 * Complejidad espacial: O(n * W) + O(n) pila de recursión.
 *
 * Ventaja sobre bottom-up: no calcula celdas innecesarias.
 * --------------------------------------------------------------- */

static int* g_weights;
static int* g_values;
static int  g_n;
static int  g_W;
static int** g_memo;

static int solve(int i, int w) {
    if (i == 0 || w == 0) return 0;
    if (g_memo[i][w] != -1) return g_memo[i][w]; /* hit de caché */

    int result;
    if (g_weights[i-1] > w) {
        result = solve(i - 1, w);
    } else {
        int sin_i = solve(i - 1, w);
        int con_i = g_values[i-1] + solve(i - 1, w - g_weights[i-1]);
        result = (con_i > sin_i) ? con_i : sin_i;
    }
    g_memo[i][w] = result;
    return result;
}

int dp_knapsack_topdown(int* weights, int* values, int n, int W,
                        int** memo) {
    g_weights = weights;
    g_values  = values;
    g_n       = n;
    g_W       = W;

    /* Inicializar tabla de memo con -1 si no se pasó externa */
    int free_memo = 0;
    if (memo == NULL) {
        memo = malloc((n + 1) * sizeof(int*));
        for (int i = 0; i <= n; i++) {
            memo[i] = malloc((W + 1) * sizeof(int));
            for (int j = 0; j <= W; j++) memo[i][j] = -1;
        }
        free_memo = 1;
    }
    g_memo = memo;

    int result = solve(n, W);

    if (free_memo) {
        for (int i = 0; i <= n; i++) free(memo[i]);
        free(memo);
    }
    return result;
}