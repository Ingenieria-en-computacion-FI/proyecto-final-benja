#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "queue.h"

void scheduler_add_process(Scheduler* s, int pid, int burst_time) {
    if (!s) return;
    
    switch (s->type) {
        case SCHED_FIFO:
            // Insertamos el PID del proceso en la cola FIFO real
            enqueue(s->fifo_queue, pid);
            printf("[Scheduler] Agregando proceso %d (Burst: %d) a FIFO\n", pid, burst_time);
            break;
            
        case SCHED_ROUND_ROBIN:
            printf("[Scheduler] Agregando proceso %d (Burst: %d) a Round Robin\n", pid, burst_time);
            break;
            
        case SCHED_SJF:
            if (s->process_count < s->capacity) {
                // Aquí ajustarás la asignación real de tu estructura Process cuando la trabajes
                s->process_count++;
            }
            printf("[Scheduler] Agregando proceso %d (Burst: %d) a SJF\n", pid, burst_time);
            break;
    }
}

void scheduler_run(Scheduler* s) {
    if (!s) return;
    printf("[Scheduler] Iniciando simulación del planificador tipo %d...\n", s->type);
}

void scheduler_destroy(Scheduler* s) {
    if (!s) return;
    
    if (s->type == SCHED_FIFO && s->fifo_queue) {
        queue_destroy(s->fifo_queue); // Activamos la liberación de la cola
    } else if (s->type == SCHED_ROUND_ROBIN && s->rr_queue) {
        // circular_queue_destroy(s->rr_queue);
    } else if (s->type == SCHED_SJF && s->processes) {
        free(s->processes);
    }
    
    free(s);
    printf("[Scheduler] Planificador liberado correctamente.\n");
}

int scheduler_next(Scheduler* s) {
    if (!s) return -1;
    
    switch (s->type) {
        case SCHED_FIFO:
            // Saca y devuelve el PID que está al frente de la cola
            return dequeue(s->fifo_queue);
            
        case SCHED_ROUND_ROBIN:
            return 0;
            
        case SCHED_SJF:
            return 0;
            
        default:
            return -1;
    }
}