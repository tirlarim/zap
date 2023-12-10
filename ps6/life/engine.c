#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <immintrin.h>
#include "engine.h"
#include "graphic.h"
#include "../utils/utils.h"

typedef struct Position {
  unsigned short startX, startY, endX, endY;
}POSITION;

typedef struct ThreadArgs {
  ARENA* arena;
  unsigned long aliveCount;
  struct Position position;
}THREAD_ARGS;

unsigned char threadsCount;

void init2DArena(ARENA* arena) {
  arena->field = (unsigned char**)malloc(arena->sizeY * sizeof(*arena->field));
  arena->buffer = (unsigned char**)malloc(arena->sizeY * sizeof(*arena->buffer));
  for (unsigned int i = 0; i < arena->sizeY; ++i) {
    arena->field[i] = (unsigned char*)malloc(arena->sizeX * sizeof(*arena->field[i]));
    arena->buffer[i] = (unsigned char*)malloc(arena->sizeX * sizeof(*arena->buffer[i]));
  }
}

void destroyArena(ARENA* arena) {
  for (int i = 0; i < arena->sizeY; ++i) {
    free(arena->field[i]);
    free(arena->buffer[i]);
  }
  free(arena->field);
  free(arena->buffer);
}

bool isArenaUnstable(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i)
    if (memcmp(arena->field[i], arena->buffer[i], arena->sizeX) != 0) return true;
  return false;
}

void copyBuffer2Arena(ARENA* arena) {
  unsigned char** tmpPtr;
  tmpPtr = arena->field;
  arena->field = arena->buffer;
  arena->buffer = tmpPtr;
}

void clearArenaBuffer(ARENA* arena, unsigned int startPos, unsigned int endPos) {
  for (unsigned int i = startPos; i < endPos; ++i)
    memset(arena->buffer[i], 0, arena->sizeX * sizeof(*arena->buffer[i]));
}

void scanArena(ARENA* arena) {
  for (unsigned int i = 0; i < arena->sizeY; ++i)
    for (unsigned int j = 0; j < arena->sizeX; ++j) {
      scanf("%hd", (short*)&arena->field[i][j]);
      if (arena->field[i][j] != 1 && arena->field[i][j] != 0) arena->field[i][j] = 1;
    }
}

void fscanArena(FILE* fp, const ARENA* restrict arena, const POINT_2D* restrict sizes) {
  for (unsigned int i = 0; i < sizes->y; ++i)
    for (unsigned int j = 0; j < sizes->x; ++j) {
      fscanf(fp, "%hd", (short*)&arena->field[i][j]);
      if (arena->field[i][j] != 1 && arena->field[i][j] != 0) arena->field[i][j] = 1;
    }
}

void* taskNative(void* args) {
  struct ThreadArgs* threadArgs = args;
  ARENA* arena = threadArgs->arena;
  POSITION* position = &threadArgs->position;
  unsigned long aliveCount = 0;
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
        if (arena->field[trueY][trueX] == 1) ++neighborsCount;
      }
      if (neighborsCount == 3 || (neighborsCount == 2 && arena->field[i][j])) {
        arena->buffer[i][j] = 1;
        ++aliveCount;
      }
    }
  }
  ((THREAD_ARGS*)args)->aliveCount = aliveCount;
  return NULL;
}

void* taskSIMD(void* args) {
  ARENA* arena = ((THREAD_ARGS*)args)->arena;
  POSITION* position = &((THREAD_ARGS*)args)->position;
  const static int offsetsArrY[DIRECTION_COUNT] __attribute__((aligned(32))) = {-1, -1, 0, 1, 1, 1, 0, -1};
  const static int offsetsArrX[DIRECTION_COUNT] __attribute__((aligned(32))) = {0, 1, 1, 1, 0, -1, -1, -1};
  int xValues[DIRECTION_COUNT] __attribute__((aligned(32))), yValues[DIRECTION_COUNT] __attribute__((aligned(32)));
  __m256i offsetsY, offsetsX, minusOne, zero, limitX, limitY, arenaSizeX, arenaSizeY;
  __m256i trueX, trueY;
  unsigned long aliveCount = 0;
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
      unsigned char neighborsCount = DIRECTION_COUNT;
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
      for (unsigned char k = 0; k != DIRECTION_COUNT; ++k) {
        if (arena->field[yValues[k]][xValues[k]] == 0) --neighborsCount;
      }
      if (neighborsCount == 3 || (neighborsCount == 2 && arena->field[i][j])) {
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
  unsigned int blocksPerThread; // possibly needs UINT64
  threadsArgs = (THREAD_ARGS*)malloc(threadsCount * sizeof(THREAD_ARGS));
  threads = (pthread_t*)malloc(threadsCount * sizeof(pthread_t));
  blocksPerThread = (arena->sizeY * arena->sizeX) / threadsCount;
  for (int i = 0; i < threadsCount; ++i) {
    unsigned int absoluteStartIndex = blocksPerThread * i;
    unsigned int absoluteEndIndex = absoluteStartIndex + blocksPerThread - 1;
    threadsArgs[i].position.startY = absoluteStartIndex / arena->sizeX;
    threadsArgs[i].position.startX = absoluteStartIndex % arena->sizeX;
    threadsArgs[i].position.endY = absoluteEndIndex / arena->sizeX;
    threadsArgs[i].position.endX = absoluteEndIndex % arena->sizeX + 1; // < n
    threadsArgs[i].arena = arena;
    threadsArgs[i].aliveCount = 0;
  }
  threadsArgs[threadsCount - 1].position.endY = arena->sizeY - 1;
  threadsArgs[threadsCount - 1].position.endX = arena->sizeX;
  arena->aliveCount = 0;
#ifdef DEBUG_LVL1
  printf("Block size: %d\n", blocksPerThread);
  for (int i = 0; i < threadsCount; ++i)
    printf("thread %d: [%d %d] -> [%d %d) | tasks: %d\n", i, positions[i].startY, positions[i].startX,
           positions[i].endY, positions[i].endX, (positions[i].endY - positions[i].startY)*arena.sizeY+(positions[i].endX-positions[i].startX));
#endif
  for (unsigned int i = 0; i < threadsCount; ++i) {
#ifdef __AVX2__
    if (pthread_create(&threads[i], NULL, &taskSIMD, &threadsArgs[i])) {
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
  copyBuffer2Arena(arena);
  clearArenaBuffer(arena, 0, arena->sizeY); // rewrite async
  free(threadsArgs);
  free(threads);
  return isAlive;
}

void play(char* mapPath) {
  ARENA arena = {};
  POINT_2D terminal = {}, reqArenaSize = {};
  FILE* inputFp = fopen(mapPath, "rt");
  unsigned int tickCount = 0;
  bool isAlive = true;
  threadsCount = getNumberOfCores();
  getTerminalSize(&terminal.y, &terminal.x);
  fscanf(inputFp, "%hd %hd", &reqArenaSize.y, &reqArenaSize.x);
  arena.sizeY = reqArenaSize.y < terminal.y ? terminal.y : reqArenaSize.y;
  arena.sizeX = reqArenaSize.x < terminal.x ? terminal.x : reqArenaSize.x;
  init2DArena(&arena);
  updateWindowInfo(&arena);
  fscanArena(inputFp, &arena, &reqArenaSize);
  fclose(inputFp);
  while (isAlive) {
    static struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    isAlive = tickArena(&arena);
    if (inputWorker(getch())) break;
    drawNewFrame(&arena, tickCount++, isAlive);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    long delta_ms = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000) / 1000;
    milliSleep((1000 - delta_ms) / FPS);
  }
  destroyArena(&arena);
}

