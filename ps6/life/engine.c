#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "graphic.h"


#define MAX_CYCLE_COUNT 1000


unsigned short normalizeIndex(short index, unsigned short limit) {
  return ((index % limit) + limit) % limit;
}

unsigned short **init2DArena(unsigned short sizeY, unsigned short sizeX) {
  unsigned short **arr =
      (unsigned short **)malloc(sizeY * sizeof(unsigned short *));
  for (unsigned short i = 0; i < sizeY; ++i) {
    arr[i] = (unsigned short *)malloc(sizeX * sizeof(unsigned short));
  }
  return arr;
}

void destroy2DArena(unsigned short **arena, unsigned short sizeY) {
  for (int i = 0; i < sizeY; ++i)
    free(arena[i]);
  free(arena);
}



bool isArenaUnstable(unsigned short **arenaOld, unsigned short **arenaNew,
                     unsigned short sizeY, unsigned short sizeX) {
  for (unsigned short i = 0; i < sizeY; ++i) {
    for (unsigned short j = 0; j < sizeX; ++j) {
      if (arenaOld[i][j] != arenaNew[i][j])
        return true;
    }
  }
  return false;
}

bool isKeyframe(unsigned int frameIndex) {
  if (frameIndex == 1 || frameIndex == 10 || frameIndex == 100 || frameIndex == 1000)
    return true;
  return false;
}

void copyArena(unsigned short **arenaSrc, unsigned short **arenaDst, unsigned short sizeY, unsigned short sizeX) {
  for (unsigned short i = 0; i < sizeY; ++i) {
    for (unsigned short j = 0; j < sizeX; ++j) {
      arenaDst[i][j] = arenaSrc[i][j];
    }
  }
}

void clearArena(unsigned short **arena, unsigned short sizeY, unsigned short sizeX) {
  for (int i = 0; i < sizeY; ++i)
    for (int j = 0; j < sizeX; ++j)
      arena[i][j] = 0;
}

void scanArena(unsigned short **arena, unsigned short sizeY, unsigned short sizeX) {
  for (unsigned short i = 0; i < sizeY; ++i)
    for (unsigned short j = 0; j < sizeX; ++j)
      scanf("%hd", &arena[i][j]);
}

void fscanArena(FILE* fp, unsigned short **arena, unsigned short sizeY, unsigned short sizeX) {
  for (unsigned short i = 0; i < sizeY; ++i)
    for (unsigned short j = 0; j < sizeX; ++j)
      fscanf(fp, "%hd", &arena[i][j]);
}

// TODO rewrite this slow shit-code
unsigned char getNeighborsCount(unsigned short **arena, unsigned short sizeY, unsigned short sizeX,
                                unsigned short positionY, unsigned short positionX) {
  unsigned char neighborsCount = 0;
  for (short i = -1; i <= 1; ++i) {
    for (short j = -1; j <= 1; ++j) {
      if (arena[normalizeIndex((short)(positionY + i), sizeY)]
               [normalizeIndex((short)(positionX + j), sizeX)] &&
          !(i == 0 && j == 0))
        ++neighborsCount;
    }
  }
  return neighborsCount;
}

bool tickArena(unsigned short **arena, unsigned short **buffer, unsigned short sizeY, unsigned short sizeX,
               unsigned int *aliveCount) {
  bool isAlive;
  *aliveCount = 0;
  for (int i = 0; i < sizeY; ++i) {
    for (int j = 0; j < sizeX; ++j) {
      unsigned char neighborsCount =
          getNeighborsCount(arena, sizeY, sizeX, i, j);
      if ((neighborsCount == 2 && arena[i][j]) || neighborsCount == 3) {
        buffer[i][j] = 1;
        ++*aliveCount;
      } else if (neighborsCount < 2 || neighborsCount > 3) {
        buffer[i][j] = 0;
      }
    }
  }
  isAlive = isArenaUnstable(arena, buffer, sizeY, sizeX) && *aliveCount;
  copyArena(buffer, arena, sizeY, sizeX);
  clearArena(buffer, sizeY, sizeX);
  return isAlive;
}

void play(char* mapPath) {
  bool isAlive = true;
  unsigned short sizeX, sizeY, terminalSizeX, terminalSizeY;
  unsigned short **arena;
  unsigned short **buffer;
  unsigned int aliveCount = 0;
  unsigned int tickCount = 0;
  FILE* inputFp = fopen(mapPath, "rt");
  fscanf(inputFp, "%hd %hd", &sizeY, &sizeX);
  getTerminalSize(&terminalSizeY, &terminalSizeX);
//  sizeY = sizeY < terminalSizeY ? terminalSizeY : sizeY;
//  sizeX = sizeX < terminalSizeX ? terminalSizeX : sizeX;
  arena = init2DArena(terminalSizeY, terminalSizeX);
  buffer = init2DArena(terminalSizeY, terminalSizeX);
  fscanArena(inputFp, arena, sizeY, sizeX);
  fclose(inputFp);
  while (isAlive) {
    isAlive = tickArena(arena, buffer, terminalSizeY, terminalSizeX, &aliveCount);
    drawNewFrame(arena, terminalSizeY, terminalSizeX, aliveCount, tickCount++, isAlive);
  }
  destroy2DArena(arena, terminalSizeY);
  destroy2DArena(buffer, terminalSizeY);
}

