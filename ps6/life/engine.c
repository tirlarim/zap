#include <stdio.h>
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

pthread_t* threads;
THREAD_ARGS* threadsArgs;

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

void threadsPrepare(ARENA* arena, SETTINGS* settings) {
  unsigned int blocksPerThread; // possibly needs UINT64
  threadsArgs = (THREAD_ARGS*)malloc(settings->threadsCount * sizeof(THREAD_ARGS));
  threads = (pthread_t*)malloc(settings->threadsCount * sizeof(pthread_t));
  blocksPerThread = (arena->sizeY * arena->sizeX) / settings->threadsCount;
  for (int i = 0; i < settings->threadsCount; ++i) {
    unsigned int absoluteStartIndex = blocksPerThread * i;
    unsigned int absoluteEndIndex = absoluteStartIndex + blocksPerThread - 1;
    threadsArgs[i].position.startY = absoluteStartIndex / arena->sizeX;
    threadsArgs[i].position.startX = absoluteStartIndex % arena->sizeX;
    threadsArgs[i].position.endY = absoluteEndIndex / arena->sizeX;
    threadsArgs[i].position.endX = absoluteEndIndex % arena->sizeX + 1; // < n
    threadsArgs[i].arena = arena;
    threadsArgs[i].aliveCount = 0;
  }
  threadsArgs[settings->threadsCount - 1].position.endY = arena->sizeY - 1;
  threadsArgs[settings->threadsCount - 1].position.endX = arena->sizeX;
}

void destroyThreadsData() {
  free(threads);
  free(threadsArgs);
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


bool tickArena(ARENA* arena, SETTINGS* settings) {
  arena->aliveCount = 0;
#ifdef DEBUG_LVL1
  printf("Block size: %d\n", blocksPerThread);
  for (int i = 0; i < threadsCount; ++i)
    printf("thread %d: [%d %d] -> [%d %d) | tasks: %d\n", i, positions[i].startY, positions[i].startX,
           positions[i].endY, positions[i].endX, (positions[i].endY - positions[i].startY)*arena.sizeY+(positions[i].endX-positions[i].startX));
#endif
  for (unsigned int i = 0; i < settings->threadsCount; ++i) {
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
  for (unsigned int i = 0; i < settings->threadsCount; ++i) {
    if (pthread_join(threads[i], NULL)) {
      perror("\nPthread join has failed\n");
      return EXIT_THREAD_JOIN_FAILURE;
    }
    arena->aliveCount+=threadsArgs[i].aliveCount;
    threadsArgs[i].aliveCount = 0;
  }
  bool isAlive = arena->aliveCount && isArenaUnstable(arena);
  copyBuffer2Arena(arena);
  clearArenaBuffer(arena, 0, arena->sizeY); // rewrite async
  return isAlive;
}

void prepareSettings(SETTINGS* settings) {
  settings->tickPerFrame = 1;
  settings->tickPerFrameMAX = UINT16_MAX;
  settings->tickPerFrameMIN = 1;
  settings->threadsCount = getNumberOfCores();
}

bool engineInputWorker(ARENA* arena, SETTINGS* settings, int key) {
  switch (key) {
    case KEY_EXIT_GAME:
      return false;
    case KEY_UP_TPF:
      if (settings->tickPerFrame != settings->tickPerFrameMAX) ++settings->tickPerFrame;
      break;
    case KEY_DOWN_TPF:
      if (settings->tickPerFrame) --settings->tickPerFrame;
      break;
    case KEY_PAUSE:
      settings->pause = !settings->pause;
      break;
    case KEY_CREATE_CELL:
      if (settings->pause) {
        POINT_2D pos;
        getCursorPos(&pos);
        arena->field[pos.y][pos.x] = !arena->field[pos.y][pos.x];
      }
      break;
    default:
      break;
  }
  return true;
}

void play(char* mapPath) {
  ARENA arena = {0};
  POINT_2D terminal = {0}, reqArenaSize = {0};
  SETTINGS settings = {0};
  FILE* inputFp = fopen(mapPath, "rt");
  struct timespec start, end, sleepTime;
  long sleepFrameTime = (1000000000 / FPS), elapsedNanosec;
  unsigned int tickCount = 0;
  int key;
  bool isAlive = true;
  sleepTime.tv_sec = 0;
  getTerminalSize(&terminal.y, &terminal.x);
  fscanf(inputFp, "%hd %hd", &reqArenaSize.y, &reqArenaSize.x);
  arena.sizeY = reqArenaSize.y < terminal.y ? terminal.y : reqArenaSize.y;
  arena.sizeX = reqArenaSize.x < terminal.x ? terminal.x : reqArenaSize.x;
  prepareSettings(&settings);
  init2DArena(&arena);
  threadsPrepare(&arena, &settings);
  updateWindowInfo(&arena);
  fscanArena(inputFp, &arena, &reqArenaSize);
  fclose(inputFp);
  while (isAlive) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    key = getch();
    for (int i = 0; !settings.pause && i < settings.tickPerFrame; ++i) isAlive = tickArena(&arena, &settings);
    if (!engineInputWorker(&arena, &settings, key)) break;
    graphicInputWorker(&arena, &settings, key);
    drawNewFrame(&arena, &settings, tickCount++, isAlive);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsedNanosec = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    sleepTime.tv_nsec = sleepFrameTime - elapsedNanosec;
    nanosleep(&sleepTime, NULL);
  }
  destroyThreadsData();
  destroyArena(&arena);
}
