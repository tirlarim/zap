#ifndef ZAP_2023_8748_ENGINE_H
#define ZAP_2023_8748_ENGINE_H

#include <stdbool.h>

#define DIRECTION_COUNT 8
#define EXIT_THREAD_CREATE_FAILURE 11
#define EXIT_THREAD_JOIN_FAILURE 12
#define KEY_UP_TPF '='
#define KEY_DOWN_TPF '-'
#define KEY_PAUSE 'q'
#define KEY_EXIT_GAME 27
#define KEY_CREATE_CELL ' '

typedef struct Arena {
  unsigned char** field __attribute__((aligned(32)));
  unsigned char** buffer __attribute__((aligned(32)));
  unsigned long aliveCount;
  int sizeY, sizeX;
}ARENA;

typedef struct Settings { // any game settings
  unsigned int tickPerFrame; // TPF
  unsigned int tickPerFrameMAX;
  unsigned int tickPerFrameMIN;
  unsigned int threadsCount;
  bool pause, drawMode;
}SETTINGS;

void play(char* mapPath);

#endif // ZAP_2023_8748_ENGINE_H
