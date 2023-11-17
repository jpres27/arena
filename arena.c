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

int main() {
    printf("%zu", sizeof(uintptr_t));
}