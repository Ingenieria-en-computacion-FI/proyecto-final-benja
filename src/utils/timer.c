#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include "utils.h"

/* ---------------------------------------------------------------
 * Timer — mide tiempo de ejecución de cada scheduler/algoritmo.
 * Usado por el benchmarking en Python.
 * --------------------------------------------------------------- */

static struct timespec _start;

void timer_start(void) {
    clock_gettime(CLOCK_MONOTONIC, &_start);
}

double timer_stop_ms(void) {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double ms = (end.tv_sec  - _start.tv_sec)  * 1000.0
              + (end.tv_nsec - _start.tv_nsec) / 1e6;
    return ms;
}