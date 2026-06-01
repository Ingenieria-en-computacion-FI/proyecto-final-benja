#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "process.h"

/* ---------------------------------------------------------------
 * Parser — lee procesos desde un archivo CSV generado por Python.
 *
 * Formato esperado (una línea por proceso):
 *   pid,burst_time,priority,memory_required
 *   1,10,2,128
 *   2,5,1,64
 *
 * Devuelve arreglo de Process y escribe el conteo en *count.
 * El caller es responsable de liberar la memoria con free().
 * --------------------------------------------------------------- */

Process* parse_processes_csv(const char* path, int* count) {
    FILE* f = fopen(path, "r");
    if (!f) { perror("parse_processes_csv"); return NULL; }

    int capacity = 64;
    Process* procs = malloc(capacity * sizeof(Process));
    *count = 0;

    char line[256];
    fgets(line, sizeof(line), f); /* saltar encabezado */

    while (fgets(line, sizeof(line), f)) {
        if (*count >= capacity) {
            capacity *= 2;
            procs = realloc(procs, capacity * sizeof(Process));
        }
        Process p;
        if (sscanf(line, "%d,%d,%d,%d",
                   &p.pid, &p.burst_time,
                   &p.priority, &p.memory_required) == 4) {
            p.remaining_time = p.burst_time;
            p.state          = READY;
            procs[(*count)++] = p;
        }
    }

    fclose(f);
    return procs;
}

/* Exporta resultados a CSV para que Python los procese */
void export_results_csv(const char* path,
                        int* pids, double* times, int n) {
    FILE* f = fopen(path, "w");
    if (!f) { perror("export_results_csv"); return; }
    fprintf(f, "pid,completion_time\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,%.4f\n", pids[i], times[i]);
    fclose(f);
}