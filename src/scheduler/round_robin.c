#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/* ---------------------------------------------------------------
 * Scheduler Round Robin
 *
 * Algoritmo:
 *   1. Cada proceso recibe como máximo `quantum` unidades de CPU.
 *   2. Si no termina, vuelve al final de la cola circular.
 *   3. Se repite hasta que todos terminen.
 *
 * Recurrencia del tiempo de espera promedio:
 *   W(n) = W(n-1) + (n-1)*q  →  W(n) = n*(n-1)/2 * q
 *
 * Complejidad temporal: O(n * ceil(burst/q)) por proceso.
 * Complejidad espacial: O(n) — cola circular + arreglo de tiempos restantes.
 * --------------------------------------------------------------- */

#define MAX_PROCESSES 256

typedef struct {
    int pid;
    int remaining;
} RREntry;

Scheduler* scheduler_create_rr(int quantum) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) { perror("scheduler_create_rr"); exit(EXIT_FAILURE); }
    s->type          = SCHED_ROUND_ROBIN;
    s->fifo_queue    = NULL;
    s->rr_queue      = cq_create(MAX_PROCESSES);
    s->processes     = malloc(sizeof(Process) * MAX_PROCESSES);
    s->process_count = 0;
    s->capacity      = MAX_PROCESSES;
    s->quantum       = quantum;
    s->time_elapsed  = 0;
    return s;
}

static void rr_add_process(Scheduler* s, int pid, int burst_time) {
    if (s->process_count >= s->capacity) {
        fprintf(stderr, "scheduler_add_process: capacidad máxima alcanzada\n");
        return;
    }
    Process p = { pid, burst_time, burst_time, 0, 0, READY };
    s->processes[s->process_count++] = p;
    cq_enqueue(s->rr_queue, s->process_count - 1); /* índice en arreglo */
}

static int rr_next(Scheduler* s) {
    if (cq_is_empty(s->rr_queue)) return -1;
    int idx = cq_dequeue(s->rr_queue);
    return s->processes[idx].pid;
}

static void rr_run(Scheduler* s) {
    printf("\n[Round Robin] quantum=%d — Iniciando simulación:\n", s->quantum);

    /* Cola de índices activa */
    CircularQueue* active = cq_create(s->process_count + 4);
    for (int i = 0; i < s->process_count; i++)
        cq_enqueue(active, i);

    while (!cq_is_empty(active)) {
        int idx = cq_dequeue(active);
        Process* p = &s->processes[idx];

        int ejecutar = (p->remaining_time < s->quantum)
                       ? p->remaining_time : s->quantum;
        p->state = RUNNING;

        for (int t = 0; t < ejecutar; t++) {
            s->time_elapsed++;
            printf("  t=%d → PID %d (restante: %d)\n",
                   s->time_elapsed, p->pid, p->remaining_time - t - 1);
        }

        p->remaining_time -= ejecutar;

        if (p->remaining_time > 0) {
            p->state = READY;
            cq_enqueue(active, idx); /* regresa al final de la cola */
        } else {
            p->state = FINISHED;
            printf("  → PID %d TERMINADO en t=%d\n", p->pid, s->time_elapsed);
        }
    }

    cq_destroy(active);
    printf("[Round Robin] Simulación terminada. Tiempo total: %d\n",
           s->time_elapsed);
}

static void rr_print_state(Scheduler* s) {
    printf("[RR] quantum=%d, procesos=%d, t=%d\n",
           s->quantum, s->process_count, s->time_elapsed);
}

static void rr_destroy(Scheduler* s) {
    if (s->rr_queue) cq_destroy(s->rr_queue);
    if (s->processes) free(s->processes);
    free(s);
}