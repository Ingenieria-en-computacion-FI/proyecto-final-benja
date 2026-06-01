#ifndef UTILS_H
#define UTILS_H

typedef enum { LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

void   logger_init(const char* path);
void   logger_log(LogLevel level, const char* fmt, ...);
void   logger_close(void);

void   timer_start(void);
double timer_stop_ms(void);

#endif

/* parser */
#include "process.h"
Process* parse_processes_csv(const char* path, int* count);
void     export_results_csv(const char* path, int* pids, double* times, int n);