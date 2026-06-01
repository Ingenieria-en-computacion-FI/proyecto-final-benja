#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"
#include "circular_queue.h"

/* ---------------------------------------------------------------
 * Scheduler — componente que decide qué proceso ejecuta el CPU.
 * Soporta FIFO, Round Robin y SJF.
 * --------------------------------------------------------------- */

typedef enum {
    SCHED_FIFO,
    SCHED_ROUND_ROBIN,
    SCHED_SJF
} SchedulerType;

typedef struct {
    SchedulerType  type;
    Queue*         fifo_queue;       /* para FIFO */
    CircularQueue* rr_queue;         /* para Round Robin */
    Process*       processes;        /* arreglo de procesos (SJF) */
    int            process_count;
    int            capacity;
    int            quantum;          /* para Round Robin */
    int            time_elapsed;     /* unidades de tiempo simuladas */
} Scheduler;

/* Constructores */
Scheduler* scheduler_create_fifo(void);
Scheduler* scheduler_create_rr(int quantum);
Scheduler* scheduler_create_sjf(void);

/* Operaciones comunes */
void scheduler_add_process(Scheduler* s, int pid, int burst_time);
int  scheduler_next(Scheduler* s);           /* devuelve PID del siguiente proceso */
void scheduler_run(Scheduler* s);            /* simula ejecución completa */
void scheduler_destroy(Scheduler* s);

/* Utilidades */
void scheduler_print_state(Scheduler* s);

#endif