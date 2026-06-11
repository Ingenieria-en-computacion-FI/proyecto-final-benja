#include <assert.h>
#include "memory_manager.h"


void test_first_fit() {
    MemoryManager* mm = mm_create(1024);

    int pid1 = mm_allocate_first_fit(mm, 100);
    int pid2 = mm_allocate_first_fit(mm, 200);

    assert(pid1 >= 0);
    assert(pid2 >= 0);

    // Guardamos dónde empezó el primer bloque (debería ser 0)
    int start_pos_block1 = mm->head->start;

    mm_free(mm, pid1);

    int pid3 = mm_allocate_first_fit(mm, 50);

    // En lugar de comparar los PIDs, validamos que el bloque recién asignado
    // haya tomado el hueco del principio de la memoria (start == 0)
    assert(mm->head->start == start_pos_block1);

    mm_destroy(mm);
}