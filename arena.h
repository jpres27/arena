#ifdef __ARENA_H__
#define __ARENA_H__

typedef struct Arena;
typedef struct PoolArena;
inline size_t align(size_t sizeToAlign);
Arena createArena(size_t bytes);
void *arenaAlloc(Arena arena, size_t size);
PoolArena createPoolArena(size_t bytesPerBlock, size_t blockNum);
void *poolArenaAlloc(PoolArena poolarena);

#endif