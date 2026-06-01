#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

/* Cola circular — usada por Round Robin.
 * Implementada con arreglo de tamaño fijo.
 * Complejidad: enqueue/dequeue O(1), espacial O(capacity). */

typedef struct {
    int* data;
    int  head;
    int  tail;
    int  size;
    int  capacity;
} CircularQueue;

CircularQueue* cq_create(int capacity);
int  cq_enqueue(CircularQueue* q, int value);
int  cq_dequeue(CircularQueue* q);
int  cq_peek(CircularQueue* q);
int  cq_is_empty(CircularQueue* q);
int  cq_is_full(CircularQueue* q);
int  cq_size(CircularQueue* q);
void cq_destroy(CircularQueue* q);

#endif