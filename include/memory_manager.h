#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

typedef struct MemoryBlock {
    int start;
    int size;
    int free;
    int pid;
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
} MemoryBlock;

typedef struct MemoryManager {
    MemoryBlock* head;
    int          total_memory;
} MemoryManager;

MemoryManager* mm_create(int total_memory);
int  mm_allocate_first_fit(MemoryManager* mm, int size);
int  mm_allocate_first_fit_pid(MemoryManager* mm, int size, int pid);
void mm_free(MemoryManager* mm, int pid);
void mm_coalesce(MemoryManager* mm);
void mm_print(MemoryManager* mm);
void mm_destroy(MemoryManager* mm);

#endif

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

typedef struct MemoryBlock {
    int start;
    int size;
    int free;
    int pid;
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
} MemoryBlock;

typedef struct MemoryManager {
    MemoryBlock* head;
    int          total_memory;
} MemoryManager;

MemoryManager* mm_create(int total_memory);
int  mm_allocate_first_fit(MemoryManager* mm, int size);
int  mm_allocate_first_fit_pid(MemoryManager* mm, int size, int pid);
void mm_free(MemoryManager* mm, int pid);
void mm_coalesce(MemoryManager* mm);
void mm_print(MemoryManager* mm);
void mm_destroy(MemoryManager* mm);

#endif