#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/* ---------------------------------------------------------------
 * Scheduler FIFO (First In, First Out)
 *
 * Algoritmo: el proceso que llegó primero se ejecuta hasta terminar.
 * No hay interrupciones ni quantum.
 *
 * Complejidad temporal: O(1) por operación de cola.
 * Complejidad espacial: O(n) donde n = número de procesos.
 * --------------------------------------------------------------- */

Scheduler* scheduler_create_fifo(void) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) { perror("scheduler_create_fifo"); exit(EXIT_FAILURE); }
    s->type          = SCHED_FIFO;
    s->fifo_queue    = queue_create();
    s->rr_queue      = NULL;
    s->processes     = NULL;
    s->process_count = 0;
    s->capacity      = 0;
    s->quantum       = 0;
    s->time_elapsed  = 0;
    return s;
}

static void fifo_add_process(Scheduler* s, int pid, int burst_time) {
    enqueue(s->fifo_queue, pid);
    (void)burst_time; /* FIFO no necesita burst_time para el orden */
    s->process_count++;
}

static int fifo_next(Scheduler* s) {
    if (queue_is_empty(s->fifo_queue)) return -1;
    return dequeue(s->fifo_queue);
}

static void fifo_run(Scheduler* s) {
    printf("\n[FIFO] Iniciando simulación:\n");
    int pid;
    while ((pid = fifo_next(s)) != -1) {
        s->time_elapsed++;
        printf("  t=%d → Ejecutando PID %d\n", s->time_elapsed, pid);
    }
    printf("[FIFO] Simulación terminada. Tiempo total: %d\n", s->time_elapsed);
}

static void fifo_print_state(Scheduler* s) {
    printf("[FIFO] Procesos en cola: %d\n", queue_size(s->fifo_queue));
}

static void fifo_destroy(Scheduler* s) {
    if (s->fifo_queue) queue_destroy(s->fifo_queue);
    free(s);
}