#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* ---------------------------------------------------------------
 * Pila (Stack) — lista enlazada con inserción/extracción por la cima.
 *
 * Complejidad temporal:
 *   push  → O(1)
 *   pop   → O(1)
 *   peek  → O(1)
 * Complejidad espacial: O(n)
 * --------------------------------------------------------------- */

struct Node {
    int          value;
    struct Node* next;
};

struct Stack {
    struct Node* top;
    int          size;
};

Stack* stack_create(void) {
    Stack* s = malloc(sizeof(Stack));
    if (!s) { perror("stack_create"); exit(EXIT_FAILURE); }
    s->top  = NULL;
    s->size = 0;
    return s;
}

void stack_push(Stack* s, int value) {
    struct Node* node = malloc(sizeof(struct Node));
    if (!node) { perror("stack_push"); exit(EXIT_FAILURE); }
    node->value = value;
    node->next  = s->top;
    s->top      = node;
    s->size++;
}

int stack_pop(Stack* s) {
    if (stack_is_empty(s)) {
        fprintf(stderr, "stack_pop: pila vacía\n");
        return -1;
    }
    struct Node* tmp = s->top;
    int val = tmp->value;
    s->top  = tmp->next;
    free(tmp);
    s->size--;
    return val;
}

int stack_peek(Stack* s) {
    if (stack_is_empty(s)) return -1;
    return s->top->value;
}

int stack_is_empty(Stack* s) {
    return s->top == NULL;
}

void stack_destroy(Stack* s) {
    while (!stack_is_empty(s)) stack_pop(s);
    free(s);
}