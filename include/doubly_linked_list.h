#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

/* Lista doblemente ligada — usada para coalescencia de bloques de memoria.
 * Navegación bidireccional necesaria para combinar bloques adyacentes.
 * Complejidad: inserción/eliminación O(1) dado nodo, búsqueda O(n). */

typedef struct DLLNode {
    int           value;
    struct DLLNode* prev;
    struct DLLNode* next;
} DLLNode;

typedef struct {
    DLLNode* head;
    DLLNode* tail;
    int      size;
} DoublyLinkedList;

DoublyLinkedList* dll_create(void);
void              dll_push_back(DoublyLinkedList* list, int value);
void              dll_push_front(DoublyLinkedList* list, int value);
void              dll_remove_node(DoublyLinkedList* list, DLLNode* node);
DLLNode*          dll_find(DoublyLinkedList* list, int value);
void              dll_print_forward(DoublyLinkedList* list);
void              dll_print_backward(DoublyLinkedList* list);
int               dll_size(DoublyLinkedList* list);
void              dll_destroy(DoublyLinkedList* list);

#endif