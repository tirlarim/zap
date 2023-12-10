#ifndef ZAP_2023_8748_GRAPHIC_H
#define ZAP_2023_8748_GRAPHIC_H

#include "engine.h"
#include <stdbool.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LOGO_PATH "./media/logo_79.txt"
#define FPS 30

#define KEY_EXIT_GAME 'q'
#define KEY_MOVE_UP 'w'
#define KEY_MOVE_DOWN 's'
#define KEY_MOVE_LEFT 'a'
#define KEY_MOVE_RIGHT 'd'
#define KEY_UP_TPF 'e'
#define KEY_DOWN_TPF 'q'

enum Colors {
  RED_ON_BLACK = 1,
  YELLOW_ON_BLACK,
  WHITE_ON_BLACK,
};

void initCurses();
void deinitCurses();
void printArena(ARENA* arena, unsigned int step);
void drawNewFrame(ARENA* arena, unsigned int tickCount, bool isAlive);
void printTestFrame();
void drawLogo();
void updateWindowInfo(ARENA* arena);
void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX);
bool inputWorker(int key); // if return false end/stop game

#endif //ZAP_2023_8748_GRAPHIC_H
