#ifndef ZAP_2023_8748_ENGINE_H
#define ZAP_2023_8748_ENGINE_H

#define DIRECTION_COUNT 8

#define EXIT_THREAD_CREATE_FAILURE 11
#define EXIT_THREAD_JOIN_FAILURE 12

typedef struct Arena {
  unsigned char** filed;
  unsigned char** buffer;
  unsigned long aliveCount;
  int sizeY, sizeX;
}ARENA;

void play(char* mapPath);

#endif // ZAP_2023_8748_ENGINE_H
