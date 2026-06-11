#include <stdio.h>

// --- Prototipos de las funciones de prueba ---
void test_stack_push_pop(void);
void test_stack_empty(void);
void test_queue_fifo(void);
void test_round_robin_behavior(void);
void test_fifo_scheduler(void);
void test_first_fit(void);

// Si en el futuro vas a usar los de abajo, ya puedes dejar sus prototipos listos:
void test_bruteforce(void);
void test_greedy(void);
void test_dp(void);

int main() {
    printf("Running integration tests...\n");

    test_stack_push_pop();
    test_stack_empty();

    test_queue_fifo();

    test_round_robin_behavior();

    test_fifo_scheduler();

    test_first_fit();

    //test_bruteforce();
    //test_greedy();
    //test_dp();

    printf("All tests passed!\n");

    return 0;
}