#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "utils.h"

/* ---------------------------------------------------------------
 * Logger — escribe eventos con timestamp a stdout y archivo de log.
 * --------------------------------------------------------------- */

static FILE* log_file = NULL;

void logger_init(const char* path) {
    log_file = fopen(path, "a");
    if (!log_file) {
        perror("logger_init");
        log_file = stderr;
    }
}

void logger_log(LogLevel level, const char* fmt, ...) {
    const char* labels[] = {"INFO", "WARN", "ERROR"};
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char ts[20];
    strftime(ts, sizeof(ts), "%H:%M:%S", t);

    va_list args;
    va_start(args, fmt);

    printf("[%s][%s] ", ts, labels[level]);
    vprintf(fmt, args);
    printf("\n");

    if (log_file && log_file != stderr) {
        va_end(args);
        va_start(args, fmt);
        fprintf(log_file, "[%s][%s] ", ts, labels[level]);
        vfprintf(log_file, fmt, args);
        fprintf(log_file, "\n");
    }

    va_end(args);
}

void logger_close(void) {
    if (log_file && log_file != stderr) fclose(log_file);
}