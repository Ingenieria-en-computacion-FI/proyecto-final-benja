#include <stdio.h>
#include <stdlib.h>
#include "circular_queue.h"

/* ---------------------------------------------------------------
 * Cola Circular — arreglo con índices head/tail que dan vuelta.
 * Usada por el scheduler Round Robin para reinserción eficiente.
 *
 * Complejidad temporal:
 *   enqueue → O(1)
 *   dequeue → O(1)
 * Complejidad espacial: O(capacity)
 *
 * Índices: tail apunta al siguiente slot libre.
 *   head = (head + 1) % capacity  tras dequeue
 *   tail = (tail + 1) % capacity  tras enqueue
 * --------------------------------------------------------------- */

CircularQueue* cq_create(int capacity) {
    CircularQueue* q = malloc(sizeof(CircularQueue));
    if (!q) { perror("cq_create"); exit(EXIT_FAILURE); }
    q->data     = malloc(sizeof(int) * capacity);
    if (!q->data) { perror("cq_create data"); exit(EXIT_FAILURE); }
    q->head     = 0;
    q->tail     = 0;
    q->size     = 0;
    q->capacity = capacity;
    return q;
}

int cq_enqueue(CircularQueue* q, int value) {
    if (cq_is_full(q)) {
        /* Redimensionar al doble */
        int new_cap  = q->capacity * 2;
        int* new_data = malloc(sizeof(int) * new_cap);
        if (!new_data) { perror("cq_enqueue realloc"); return -1; }
        for (int i = 0; i < q->size; i++)
            new_data[i] = q->data[(q->head + i) % q->capacity];
        free(q->data);
        q->data     = new_data;
        q->head     = 0;
        q->tail     = q->size;
        q->capacity = new_cap;
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return 0;
}

int cq_dequeue(CircularQueue* q) {
    if (cq_is_empty(q)) {
        fprintf(stderr, "cq_dequeue: cola circular vacía\n");
        return -1;
    }
    int val  = q->data[q->head];
    q->head  = (q->head + 1) % q->capacity;
    q->size--;
    return val;
}

int cq_peek(CircularQueue* q) {
    if (cq_is_empty(q)) return -1;
    return q->data[q->head];
}

int cq_is_empty(CircularQueue* q) { return q->size == 0; }
int cq_is_full(CircularQueue* q)  { return q->size == q->capacity; }
int cq_size(CircularQueue* q)     { return q->size; }

void cq_destroy(CircularQueue* q) {
    free(q->data);
    free(q);
}