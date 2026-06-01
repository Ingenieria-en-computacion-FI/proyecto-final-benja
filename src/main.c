#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "algorithms.h"
#include "utils.h"

/* ---------------------------------------------------------------
 * Mini OS — simulador de sistema operativo.
 *
 * Uso: ./bin/main [num_procesos]
 *   num_procesos: cuántos procesos generar (default: 5)
 *
 * Flujo:
 *   1. Generar procesos
 *   2. Demostrar schedulers (FIFO, Round Robin, SJF)
 *   3. Demostrar administración de memoria (First/Best/Worst Fit)
 *   4. Demostrar coalescencia y compactación
 *   5. Exportar resultados para Python
 * --------------------------------------------------------------- */

static void demo_schedulers(int n, Process* procs) {
    printf("\n=== SCHEDULERS ===\n");

    /* FIFO */
    Scheduler* fifo = scheduler_create_fifo();
    for (int i = 0; i < n; i++)
        scheduler_add_process(fifo, procs[i].pid, procs[i].burst_time);
    scheduler_run(fifo);
    scheduler_destroy(fifo);

    /* Round Robin con quantum=3 */
    Scheduler* rr = scheduler_create_rr(3);
    for (int i = 0; i < n; i++)
        scheduler_add_process(rr, procs[i].pid, procs[i].burst_time);
    scheduler_run(rr);
    scheduler_destroy(rr);

    /* SJF */
    Scheduler* sjf = scheduler_create_sjf();
    for (int i = 0; i < n; i++)
        scheduler_add_process(sjf, procs[i].pid, procs[i].burst_time);
    scheduler_run(sjf);
    scheduler_destroy(sjf);
}

static void demo_memory(int n, Process* procs) {
    printf("\n=== ADMINISTRACIÓN DE MEMORIA ===\n");

    MemoryManager* mm = mm_create(1024);

    printf("\n-- First Fit --\n");
    for (int i = 0; i < n && i < 4; i++) {
        int pid = mm_allocate_first_fit_pid(mm, procs[i].memory_required,
                                            procs[i].pid);
        printf("  Asignado PID=%d size=%d → %s\n",
               procs[i].pid, procs[i].memory_required,
               pid != -1 ? "OK" : "FALLO");
    }
    mm_print(mm);

    printf("\n-- Liberando PID=%d y coalescencia --\n", procs[1].pid);
    mm_free(mm, procs[1].pid);
    mm_print(mm);

    mm_destroy(mm);

    /* Best Fit */
    printf("\n-- Best Fit --\n");
    mm = mm_create(1024);
    for (int i = 0; i < n && i < 3; i++)
        mm_allocate_best_fit(mm, procs[i].memory_required, procs[i].pid);
    mm_print(mm);
    mm_destroy(mm);
}

int main(int argc, char* argv[]) {
    logger_init("data/logs/mini_os.log");
    logger_log(LOG_INFO, "Mini OS iniciado");

    int n = (argc > 1) ? atoi(argv[1]) : 5;
    if (n <= 0 || n > 100) n = 5;

    /* Generar procesos de prueba */
    Process procs[100];
    int burst_times[]  = {5, 3, 8, 2, 6, 4, 7, 1, 9, 10};
    int mem_required[] = {128, 64, 256, 32, 100, 200, 150, 80, 300, 50};

    for (int i = 0; i < n; i++) {
        procs[i].pid             = i + 1;
        procs[i].burst_time      = burst_times[i % 10];
        procs[i].remaining_time  = procs[i].burst_time;
        procs[i].priority        = (i % 5) + 1;
        procs[i].memory_required = mem_required[i % 10];
        procs[i].state           = READY;
    }

    printf("Mini OS — %d procesos\n", n);

    timer_start();
    demo_schedulers(n, procs);
    double sched_ms = timer_stop_ms();
    printf("\n[timer] Schedulers: %.3f ms\n", sched_ms);

    timer_start();
    demo_memory(n, procs);
    double mem_ms = timer_stop_ms();
    printf("\n[timer] Memoria: %.3f ms\n", mem_ms);

    /* Exportar tiempos para Python */
    int pids[2] = {1, 2};
    double times[2] = {sched_ms, mem_ms};
    export_results_csv("data/outputs/results.csv", pids, times, 2);

    logger_log(LOG_INFO, "Mini OS terminado");
    logger_close();
    return 0;
}