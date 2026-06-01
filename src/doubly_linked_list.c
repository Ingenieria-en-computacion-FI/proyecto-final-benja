#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

/* ---------------------------------------------------------------
 * Lista doblemente ligada — navegación bidireccional.
 * Usada internamente por el administrador de memoria para
 * recorrer bloques en ambas direcciones durante la coalescencia.
 *
 * Complejidad temporal:
 *   push_front/push_back → O(1)  (apuntadores a head y tail)
 *   remove_node          → O(1)  (dado el nodo, religa prev/next)
 *   find                 → O(n)
 * Complejidad espacial: O(n)
 * --------------------------------------------------------------- */

DoublyLinkedList* dll_create(void) {
    DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));
    if (!list) { perror("dll_create"); exit(EXIT_FAILURE); }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dll_push_back(DoublyLinkedList* list, int value) {
    DLLNode* node = malloc(sizeof(DLLNode));
    if (!node) { perror("dll_push_back"); exit(EXIT_FAILURE); }
    node->value = value;
    node->next  = NULL;
    node->prev  = list->tail;

    if (list->tail) list->tail->next = node;
    else            list->head       = node;

    list->tail = node;
    list->size++;
}

void dll_push_front(DoublyLinkedList* list, int value) {
    DLLNode* node = malloc(sizeof(DLLNode));
    if (!node) { perror("dll_push_front"); exit(EXIT_FAILURE); }
    node->value = value;
    node->prev  = NULL;
    node->next  = list->head;

    if (list->head) list->head->prev = node;
    else            list->tail       = node;

    list->head = node;
    list->size++;
}

void dll_remove_node(DoublyLinkedList* list, DLLNode* node) {
    if (!node) return;

    if (node->prev) node->prev->next = node->next;
    else            list->head       = node->next;

    if (node->next) node->next->prev = node->prev;
    else            list->tail       = node->prev;

    free(node);
    list->size--;
}

DLLNode* dll_find(DoublyLinkedList* list, int value) {
    DLLNode* cur = list->head;
    while (cur) {
        if (cur->value == value) return cur;
        cur = cur->next;
    }
    return NULL;
}

void dll_print_forward(DoublyLinkedList* list) {
    DLLNode* cur = list->head;
    printf("[DLL →]: ");
    while (cur) { printf("%d ", cur->value); cur = cur->next; }
    printf("\n");
}

void dll_print_backward(DoublyLinkedList* list) {
    DLLNode* cur = list->tail;
    printf("[DLL ←]: ");
    while (cur) { printf("%d ", cur->value); cur = cur->prev; }
    printf("\n");
}

int dll_size(DoublyLinkedList* list) { return list->size; }

void dll_destroy(DoublyLinkedList* list) {
    DLLNode* cur = list->head;
    while (cur) {
        DLLNode* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(list);
}