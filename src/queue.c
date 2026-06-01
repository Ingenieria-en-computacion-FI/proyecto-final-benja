#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* ---------------------------------------------------------------
 * Cola FIFO — lista enlazada con inserción por rear, extracción por front.
 *
 * Complejidad temporal:
 *   enqueue → O(1)  (apuntador a rear, no se recorre la lista)
 *   dequeue → O(1)
 *   peek    → O(1)
 * Complejidad espacial: O(n)
 * --------------------------------------------------------------- */

Queue* queue_create(void) {
    Queue* q = malloc(sizeof(Queue));
    if (!q) { perror("queue_create"); exit(EXIT_FAILURE); }
    q->front = NULL;
    q->rear  = NULL;
    q->size  = 0;
    return q;
}

void enqueue(Queue* q, int value) {
    QueueNode* node = malloc(sizeof(QueueNode));
    if (!node) { perror("enqueue"); exit(EXIT_FAILURE); }
    node->value = value;
    node->next  = NULL;

    if (q->rear == NULL) {
        q->front = node;
        q->rear  = node;
    } else {
        q->rear->next = node;
        q->rear       = node;
    }
    q->size++;
}

int dequeue(Queue* q) {
    if (queue_is_empty(q)) {
        fprintf(stderr, "dequeue: cola vacía\n");
        return -1;
    }
    QueueNode* tmp = q->front;
    int val = tmp->value;
    q->front = tmp->next;
    if (q->front == NULL) q->rear = NULL;
    free(tmp);
    q->size--;
    return val;
}

int queue_peek(Queue* q) {
    if (queue_is_empty(q)) return -1;
    return q->front->value;
}

int queue_is_empty(Queue* q) {
    return q->front == NULL;
}

int queue_size(Queue* q) {
    return q->size;
}

void queue_destroy(Queue* q) {
    while (!queue_is_empty(q)) dequeue(q);
    free(q);
}