#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "process.h"

/* Lista simplemente ligada — almacena procesos terminados ordenados por PID.
 * Complejidad: inserción ordenada O(n), búsqueda O(n), espacial O(n). */

typedef struct ListNode {
    Process          process;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    int       size;
} LinkedList;

LinkedList* ll_create(void);
void        ll_insert_sorted(LinkedList* list, Process p);   /* inserta por PID ascendente */
Process*    ll_find(LinkedList* list, int pid);
void        ll_remove(LinkedList* list, int pid);
void        ll_print(LinkedList* list);
int         ll_size(LinkedList* list);
void        ll_destroy(LinkedList* list);

#endif