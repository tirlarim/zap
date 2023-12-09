#ifndef ZAP_2023_8748_GRAPHIC_H
#define ZAP_2023_8748_GRAPHIC_H

#include "engine.h"
#include <stdbool.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LOGO_PATH "./media/logo_79.txt"
#define FPS 30

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
void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX);

#endif //ZAP_2023_8748_GRAPHIC_H
