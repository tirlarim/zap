#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <pthread.h>
#include "engine.h"
#include "../utils/utils.h"
#include "graphic.h"

typedef struct Position {
  unsigned short startX, startY, endX, endY;
}POSITION;

typedef struct ThreadArgs {
  ARENA* arena;
  POSITION* position;
  unsigned long aliveCount;
}THREAD_ARGS;

typedef struct Point2D {
  unsigned short y, x;
}POINT_2D;

unsigned char threadsCount;

void init2DArena(ARENA* arena) {
  arena->filed = (unsigned char**)malloc(arena->sizeY * sizeof(*arena->filed));
  arena->buffer = (unsigned char**)malloc(arena->sizeY * sizeof(*arena->buffer));
  for (unsigned int i = 0; i < arena->sizeY; ++i) {
    arena->filed[i] = (unsigned char*)malloc(arena->sizeX * sizeof(*arena->filed[i]));
    arena->buffer[i] = (unsigned char*)malloc(arena->sizeX * sizeof(*arena->buffer[i]));
  }
}

void destroyArena(ARENA* arena) {
  for (int i = 0; i < arena->sizeY; ++i) {
    free(arena->filed[i]);
    free(arena->buffer[i]);
  }
  free(arena->filed);
  free(arena->buffer);
}

bool isArenaUnstable(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i)
    if (memcmp(arena->filed[i], arena->buffer[i], arena->sizeX) != 0) return true;
  return false;
}

void copyArena(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i) {
    memcpy(arena->filed[i], arena->buffer[i], arena->sizeX);
  }
}

void clearArenaBuffer(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i)
    memset(arena->buffer[i], 0, arena->sizeX * sizeof(*arena->buffer[i]));
}

void scanArena(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i)
    for (unsigned int j = 0; j < arena->sizeX; ++j) {
      scanf("%hd", (short*)&arena->filed[i][j]);
      if (arena->filed[i][j] != 1 && arena->filed[i][j] != 0) arena->filed[i][j] = 1;
    }
}

void fscanArena(FILE* fp, const ARENA* restrict arena, const POINT_2D* restrict sizes) {
  for (unsigned int i = 0; i < sizes->y; ++i)
    for (unsigned int j = 0; j < sizes->x; ++j) {
      fscanf(fp, "%hd", (short*) &arena->filed[i][j]);
      if (arena->filed[i][j] != 1 && arena->filed[i][j] != 0) arena->filed[i][j] = 1;
    }
}

void* taskNative(void* args) {
  struct ThreadArgs* threadArgs = args;
  ARENA* arena = threadArgs->arena;
  POSITION* position = threadArgs->position;
  unsigned char neighborsCount;
  const static int offsets[DIRECTION_COUNT][2] = { // y x
      {-1, 0},
      {-1, 1},
      {0,  1},
      {1,  1},
      {1,  0},
      {1,  -1},
      {0,  -1},
      {-1, -1},
  };
  for (int i = position->startY; i <= position->endY; ++i) {
    int startPos = i != position->startY ? 0 : position->startX;
    int posEnd = i != position->endY ? arena->sizeX : position->endX;
    for (int j = startPos; j < posEnd; ++j) {
      neighborsCount = 0;
      for (unsigned char k = 0; k < DIRECTION_COUNT; ++k) {
        int trueY = i + offsets[k][1];
        int trueX = j + offsets[k][0];
        if (trueX == -1) trueX = arena->sizeX - 1;
        else if (trueX == arena->sizeX) trueX = 0;
        if (trueY == -1) trueY = arena->sizeY - 1;
        else if (trueY == arena->sizeY) trueY = 0;
        if (arena->filed[trueY][trueX] == 1) ++neighborsCount;
      }
      if ((neighborsCount == 2 && arena->filed[i][j]) || neighborsCount == 3) {
        arena->buffer[i][j] = 1;
      } else if (neighborsCount < 2 || neighborsCount > 3) {
        arena->buffer[i][j] = 0;
      }
    }
  }
  return NULL;
}

void* taskSIMD(void* args) {
  ARENA* arena = ((THREAD_ARGS*) args)->arena;
  POSITION* position = ((THREAD_ARGS*) args)->position;
  const static int offsetsArrY[DIRECTION_COUNT] __attribute__((aligned(32))) = {-1, -1, 0, 1, 1, 1, 0, -1};
  const static int offsetsArrX[DIRECTION_COUNT] __attribute__((aligned(32))) = {0, 1, 1, 1, 0, -1, -1, -1};
  int xValues[DIRECTION_COUNT] __attribute__((aligned(32))), yValues[DIRECTION_COUNT] __attribute__((aligned(32)));
  __m256i offsetsY, offsetsX, minusOne, zero, limitX, limitY, arenaSizeX, arenaSizeY;
  __m256i trueX, trueY;
  unsigned long aliveCount = 0;
  unsigned char neighborsCount = 0;
  offsetsY = _mm256_load_si256((const __m256i*) offsetsArrY);
  offsetsX = _mm256_load_si256((const __m256i*) offsetsArrX);
  arenaSizeX = _mm256_set1_epi32(arena->sizeX);
  arenaSizeY = _mm256_set1_epi32(arena->sizeY);
  limitX = _mm256_set1_epi32(arena->sizeX - 1);
  limitY = _mm256_set1_epi32(arena->sizeY - 1);
  minusOne = _mm256_set1_epi32(-1);
  zero = _mm256_setzero_si256();
  for (int i = position->startY; i <= position->endY; ++i) {
    int startPos = i != position->startY ? 0 : position->startX;
    int posEnd = i != position->endY ? arena->sizeX : position->endX;
    for (int j = startPos; j < posEnd; ++j) {
      neighborsCount = DIRECTION_COUNT;
      trueX = _mm256_add_epi32(_mm256_set1_epi32(j), offsetsX);
      trueY = _mm256_add_epi32(_mm256_set1_epi32(i), offsetsY);
      trueX = _mm256_blendv_epi8(trueX, zero, _mm256_cmpeq_epi32(trueX, arenaSizeX));
      trueY = _mm256_blendv_epi8(trueY, zero, _mm256_cmpeq_epi32(trueY, arenaSizeY));
      trueX = _mm256_blendv_epi8(trueX, limitX, _mm256_cmpeq_epi32(trueX, minusOne));
      trueY = _mm256_blendv_epi8(trueY, limitY, _mm256_cmpeq_epi32(trueY, minusOne));
      _mm256_store_si256((__m256i*)xValues, trueX);
      _mm256_store_si256((__m256i*)yValues, trueY);
//    I have no idea how to make it FASTER. any help?
//    arena->filed is big and stored in heap, as a result works slow.
      for (unsigned char k = 0; k != DIRECTION_COUNT; ++k)
        if (arena->filed[yValues[k]][xValues[k]] == 0) --neighborsCount;
      if (neighborsCount < 2 || neighborsCount > 3) {
        arena->buffer[i][j] = 0;
      } else if (neighborsCount == 3 || (neighborsCount == 2 && arena->filed[i][j])) {
        arena->buffer[i][j] = 1;
        ++aliveCount;
      }
    }
  }
#ifdef DEBUG_LVL2
  printf("finish work [%d %d]\n", position->startY, position->startX);
#endif
  ((THREAD_ARGS*) args)->aliveCount = aliveCount;
  return NULL;
}


bool tickArena(ARENA* arena) {
  pthread_t* threads;
  THREAD_ARGS* threadsArgs;
  POSITION* positions;
  unsigned int blocksPerThread, blockAll = arena->sizeY * arena->sizeX; // possibly needs UINT64, UINT32
  positions = (POSITION*)malloc(threadsCount * sizeof(POSITION)); // rewrite fix allocate/deallocate mem in cycle
  threadsArgs = (THREAD_ARGS*)malloc(threadsCount * sizeof(THREAD_ARGS));
  threads = (pthread_t*)malloc(threadsCount * sizeof(pthread_t));
  blocksPerThread = blockAll / threadsCount;
  for (int i = 0; i < threadsCount; ++i) {
    unsigned int absoluteIndex = (blocksPerThread * i);
    positions[i].startY = absoluteIndex / arena->sizeX;
    positions[i].startX = absoluteIndex % arena->sizeX;
    positions[i].endY = (blocksPerThread * (i + 1) - 1) / arena->sizeX;
    positions[i].endX = (blocksPerThread * (i + 1) - 1) % arena->sizeX + 1; // < n
  }
  positions[threadsCount - 1].endY = arena->sizeY - 1;
  positions[threadsCount - 1].endX = arena->sizeX;
  arena->aliveCount = 0;
#ifdef DEBUG_LVL1
  printf("Block size: %d\n", blocksPerThread);
  for (int i = 0; i < threadsCount; ++i)
    printf("thread %d: [%d %d] -> [%d %d) | tasks: %d\n", i, positions[i].startY, positions[i].startX,
           positions[i].endY, positions[i].endX, (positions[i].endY - positions[i].startY)*arena.sizeY+(positions[i].endX-positions[i].startX));
#endif
  for (unsigned int j = 0; j < threadsCount; ++j) {
    threadsArgs[j].arena = arena;
    threadsArgs[j].position = &positions[j];
    threadsArgs[j].aliveCount = 0;
#ifdef __AVX2__
    if (pthread_create(&threads[j], NULL, &taskSIMD, &threadsArgs[j])) {
      perror("\nPthread create has failed\n");
      return EXIT_THREAD_CREATE_FAILURE;
    }
#else
    if (pthread_create(&threads[j], NULL, &taskNative, &threadsArgs[j])) {
      perror("\nPthread create has failed\n");
      return EXIT_THREAD_CREATE_FAILURE;
    }
#endif
  }
  for (unsigned int i = 0; i < threadsCount; ++i) {
    if (pthread_join(threads[i], NULL)) {
      perror("\nPthread join has failed\n");
      return EXIT_THREAD_JOIN_FAILURE;
    }
    arena->aliveCount+=threadsArgs[i].aliveCount;
  }
  bool isAlive = arena->aliveCount && isArenaUnstable(arena);
  copyArena(arena);
  clearArenaBuffer(arena);
  free(threadsArgs);
  free(threads);
  free(positions);
  return isAlive;
}

void play(char* mapPath) {
  ARENA arena = {};
  POINT_2D terminal = {}, reqArenaSize = {};
  FILE* inputFp = fopen(mapPath, "rt");
  unsigned int tickCount = 0;
  bool isAlive = true;
  getTerminalSize(&terminal.y, &terminal.x);
  fscanf(inputFp, "%hd %hd", &reqArenaSize.y, &reqArenaSize.x);
  arena.sizeY = reqArenaSize.y < terminal.y ? terminal.y : reqArenaSize.y;
  arena.sizeX = reqArenaSize.x < terminal.x ? terminal.x : reqArenaSize.x;
  init2DArena(&arena);
  fscanArena(inputFp, &arena, &reqArenaSize);
  fclose(inputFp);
  while (isAlive && getch() != 'q') {
    isAlive = tickArena(&arena);
    drawNewFrame(&arena, tickCount++, isAlive);
  }
  destroyArena(&arena);
}

