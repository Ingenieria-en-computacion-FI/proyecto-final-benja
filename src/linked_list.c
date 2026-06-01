#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/* ---------------------------------------------------------------
 * Lista simplemente ligada — almacena procesos TERMINADOS ordenados por PID.
 *
 * Invariante: la lista siempre está ordenada por pid ascendente.
 *
 * Complejidad temporal:
 *   insert_sorted → O(n)  (recorre hasta encontrar posición)
 *   find          → O(n)
 *   remove        → O(n)
 * Complejidad espacial: O(n)
 * --------------------------------------------------------------- */

LinkedList* ll_create(void) {
    LinkedList* list = malloc(sizeof(LinkedList));
    if (!list) { perror("ll_create"); exit(EXIT_FAILURE); }
    list->head = NULL;
    list->size = 0;
    return list;
}

void ll_insert_sorted(LinkedList* list, Process p) {
    ListNode* new_node = malloc(sizeof(ListNode));
    if (!new_node) { perror("ll_insert_sorted"); exit(EXIT_FAILURE); }
    new_node->process = p;
    new_node->next    = NULL;

    /* Insertar antes del primer nodo con pid mayor */
    if (list->head == NULL || list->head->process.pid >= p.pid) {
        new_node->next = list->head;
        list->head     = new_node;
        list->size++;
        return;
    }

    ListNode* cur = list->head;
    while (cur->next != NULL && cur->next->process.pid < p.pid)
        cur = cur->next;

    new_node->next = cur->next;
    cur->next      = new_node;
    list->size++;
}

Process* ll_find(LinkedList* list, int pid) {
    ListNode* cur = list->head;
    while (cur != NULL) {
        if (cur->process.pid == pid) return &cur->process;
        cur = cur->next;
    }
    return NULL;
}

void ll_remove(LinkedList* list, int pid) {
    if (list->head == NULL) return;

    if (list->head->process.pid == pid) {
        ListNode* tmp = list->head;
        list->head = tmp->next;
        free(tmp);
        list->size--;
        return;
    }

    ListNode* cur = list->head;
    while (cur->next != NULL && cur->next->process.pid != pid)
        cur = cur->next;

    if (cur->next != NULL) {
        ListNode* tmp = cur->next;
        cur->next = tmp->next;
        free(tmp);
        list->size--;
    }
}

void ll_print(LinkedList* list) {
    ListNode* cur = list->head;
    printf("[Lista procesos terminados]:\n");
    while (cur != NULL) {
        printf("  PID=%d burst=%d\n",
               cur->process.pid, cur->process.burst_time);
        cur = cur->next;
    }
}

int ll_size(LinkedList* list) { return list->size; }

void ll_destroy(LinkedList* list) {
    ListNode* cur = list->head;
    while (cur != NULL) {
        ListNode* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(list);
}