CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Fuentes: listar subdirectorios explícitamente
# (src/**/*.c no funciona con GNU make en todos los entornos)
SRC_SCHED  = src/scheduler/fifo.c src/scheduler/round_robin.c src/scheduler/sjf.c
SRC_MEM    = src/memory/first_fit.c src/memory/coalescence.c \
             src/memory/best_fit.c src/memory/compactation.c
SRC_ALGO   = src/algorithms/brute_force.c src/algorithms/greedy.c \
             src/algorithms/backtracking.c src/algorithms/divide_conquer.c \
             src/algorithms/dp_bottomup.c src/algorithms/dp_topdown.c \
             src/algorithms/incremental.c
SRC_DS     = src/stack.c src/queue.c src/circular_queue.c \
             src/linked_list.c src/doubly_linked_list.c src/scheduler.c
SRC_UTILS  = src/utils/logger.c src/utils/timer.c src/utils/parser.c
SRC_MAIN   = src/main.c

SRC_ALL = $(SRC_DS) $(SRC_SCHED) $(SRC_MEM) $(SRC_ALGO) $(SRC_UTILS)
TESTS   = tests/test_stack.c tests/test_queue.c tests/test_circular_queue.c \
          tests/test_memory_manager.c tests/test_scheduler.c tests/main.c

all: bin/main

bin/main: $(SRC_ALL) $(SRC_MAIN)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

run: bin/main
	./bin/main

test: $(SRC_ALL) $(TESTS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o bin/tests
	./bin/tests

benchmark: bin/main
	@mkdir -p reports/csv reports/png data/outputs
	python3 scripts/generate_processes.py --n 20
	python3 scripts/benchmark.py
	python3 scripts/graphs.py

validate: benchmark
	python3 scripts/validate_results.py

clean:
	rm -rf bin/*

.PHONY: all run test benchmark validate clean