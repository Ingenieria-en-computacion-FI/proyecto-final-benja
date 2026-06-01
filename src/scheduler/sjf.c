#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/* ---------------------------------------------------------------
 * Scheduler SJF (Shortest Job First)
 *
 * Algoritmo greedy: siempre ejecuta el proceso con menor burst_time.
 * Minimiza el tiempo de espera promedio (óptimo no apropiativo).
 *
 * Implementación: insertion sort al agregar → mantiene el arreglo
 * ordenado por burst_time ascendente en todo momento.
 *
 * Recurrencia del tiempo de espera:
 *   W(0)=0, W(i) = W(i-1) + b_{i-1}
 *   donde b_0 ≤ b_1 ≤ ... ≤ b_{n-1} (burst times ordenados)
 *
 * Complejidad temporal:
 *   add_process → O(n)  insertion sort
 *   next        → O(1)  siempre el primero del arreglo
 *   run         → O(n²) total (n inserciones × O(n) cada una)
 * Complejidad espacial: O(n)
 * --------------------------------------------------------------- */

#define MAX_PROCESSES_SJF 256

Scheduler* scheduler_create_sjf(void) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) { perror("scheduler_create_sjf"); exit(EXIT_FAILURE); }
    s->type          = SCHED_SJF;
    s->fifo_queue    = NULL;
    s->rr_queue      = NULL;
    s->processes     = malloc(sizeof(Process) * MAX_PROCESSES_SJF);
    s->process_count = 0;
    s->capacity      = MAX_PROCESSES_SJF;
    s->quantum       = 0;
    s->time_elapsed  = 0;
    return s;
}

static void sjf_add_process(Scheduler* s, int pid, int burst_time) {
    if (s->process_count >= s->capacity) {
        fprintf(stderr, "scheduler_add_process SJF: capacidad máxima\n");
        return;
    }

    Process p = { pid, burst_time, burst_time, 0, 0, READY };
    int i = s->process_count - 1;

    /* Insertion sort: desplaza hacia la derecha mientras burst sea mayor */
    while (i >= 0 && s->processes[i].burst_time > burst_time) {
        s->processes[i + 1] = s->processes[i];
        i--;
    }
    s->processes[i + 1] = p;
    s->process_count++;
}

static int sjf_next(Scheduler* s) {
    if (s->process_count == 0) return -1;

    /* El primer elemento ya es el de menor burst_time */
    int pid = s->processes[0].pid;

    /* Desplazar arreglo una posición a la izquierda */
    for (int i = 0; i < s->process_count - 1; i++)
        s->processes[i] = s->processes[i + 1];
    s->process_count--;

    return pid;
}

static void sjf_run(Scheduler* s) {
    printf("\n[SJF] Iniciando simulación:\n");
    int pid;
    while ((pid = sjf_next(s)) != -1) {
        s->time_elapsed++;
        printf("  t=%d → Ejecutando PID %d\n", s->time_elapsed, pid);
    }
    printf("[SJF] Simulación terminada. Tiempo total: %d\n", s->time_elapsed);
}

static void sjf_print_state(Scheduler* s) {
    printf("[SJF] Procesos pendientes: %d\n", s->process_count);
    for (int i = 0; i < s->process_count; i++)
        printf("  [%d] PID=%d burst=%d\n", i,
               s->processes[i].pid, s->processes[i].burst_time);
}

static void sjf_destroy(Scheduler* s) {
    if (s->processes) free(s->processes);
    free(s);
}