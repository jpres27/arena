#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "arena.h"


typedef struct {
    void *position;
    size_t bytes;
} Arena;

typedef struct {
    void *position;
    uint_fast32_t blockCounter;     // Incremented every time a block is allocated
    size_t bytes;
} PoolArena;

inline size_t align(size_t sizeToAlign) {
    return (sizeToAlign + sizeof(uintptr_t) - 1) & ~(sizeof(uintptr_t) -1);
}

// Create arena, at least as large as the arg bytes
Arena createArena(size_t bytes) {
    void *x;
    x = mmap(0, bytes, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    Arena arena;
    if (x == MAP_FAILED) {
        printf("createArena() failed to map requested memory. %s\n", strerror(errno));
        arena.bytes = 0;
        return arena;
    }
    arena.position = x;
    arena.bytes = bytes;
    return arena;
}

// Create an arena with uniformly sized blocks of memory
// For now, caller is responsible for maintaining a free list
Arena createPoolArena(size_t bytesPerBlock, size_t blockNum) {
    size_t bytes = blockNum * (align(bytesPerBlock));
    void *x; 
    x = mmap(0, bytes, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    PoolArena poolarena;
    poolarena.position = x;
    poolarena.blockCounter = 0;
    poolarena.bytes = bytes;

}

int main() {
    printf("%zu", sizeof(uintptr_t));
    return 0;
}