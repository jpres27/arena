#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "arena.h"


typedef struct {
    void *memory;
    size_t capacity;
    size_t position;
} Arena;

typedef struct {
    void *memory;
    uint_fast32_t blockCounter;     // Incremented every time a block is allocated
    size_t capacity;
    size_t position;
    size_t blockSize;
} PoolArena;

inline size_t align(size_t sizeToAlign) {
    return (sizeToAlign + sizeof(uintptr_t) - 1) & ~(sizeof(uintptr_t) -1);
}

// Create arena, at least as large as 'bytes'
Arena createArena(size_t bytes) {
    void *x;
    bytes = align(bytes);
    x = mmap(0, bytes, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    Arena arena;
    if (x == MAP_FAILED) {
        printf("createArena() failed to map requested memory. %s\n", strerror(errno));
        arena.capacity = 0;
        return arena;
    }
    arena.memory = x;
    arena.position = 0;
    arena.capacity = bytes;
    return arena;
}

void *arenaAlloc(Arena arena, size_t size) {
    size = align(size);
    if (arena.position + size > arena.capacity) return NULL;
    void *x = ((uint8_t *) arena.memory) + arena.position;
    arena.position += size;
    return x;
}

// Create an arena with uniformly sized blocks of memory
// For now, caller is responsible for maintaining a free list
PoolArena createPoolArena(size_t bytesPerBlock, size_t blockNum) {
    size_t blockSize = align(bytesPerBlock);
    size_t bytes = blockNum * blockSize;
    void *x; 
    x = mmap(0, bytes, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    PoolArena poolarena;
    if (x == MAP_FAILED) {
        printf("createArena() failed to map requested memory. %s\n", strerror(errno));
        poolarena.capacity = 0;
        return poolarena;
    }
    poolarena.memory = x;
    poolarena.position = 0;
    poolarena.blockCounter = 0;
    poolarena.capacity = bytes;
    poolarena.blockSize = blockSize;
    return poolarena;
}

void *poolArenaAlloc(PoolArena poolarena) {
    if (poolarena.position + poolarena.blockSize > poolarena.capacity) return NULL;
    void *x = ((uint8_t *) poolarena.memory) + poolarena.position;
    poolarena.position += poolarena.blockSize;
    poolarena.blockCounter++;
    return x;
}

int main() {
    printf("%zu", sizeof(uintptr_t));
    return 0;
}