#ifndef QUEUE_H
#define QUEUE_H

/* Cola simple FIFO — usada por el scheduler FIFO.
 * Complejidad: enqueue O(1), dequeue O(1), espacial O(n). */

typedef struct QueueNode {
    int value;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

Queue* queue_create(void);
void   enqueue(Queue* q, int value);
int    dequeue(Queue* q);
int    queue_peek(Queue* q);
int    queue_is_empty(Queue* q);
int    queue_size(Queue* q);
void   queue_destroy(Queue* q);

#endif