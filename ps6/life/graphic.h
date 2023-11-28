#ifndef ZAP_2023_8748_GRAPHIC_H
#define ZAP_2023_8748_GRAPHIC_H

#define FPS 30
#define CURSES_ALLOWED
#include <stdbool.h>
#ifdef CURSES_ALLOWED
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#else
#include <stdio.h>
#endif

#define LOGO_PATH "/Users/stanislavromanov/CLionProjects/TUKE/zap-2023-8748/ps6/media/logo_79.txt"

enum Colors {
  RED_ON_BLACK = 1,
  YELLOW_ON_BLACK,
  WHITE_ON_BLACK,
};

void initCurses();
void deinitCurses();
void printArena(unsigned short **arena, unsigned short sizeY, unsigned short sizeX,
                unsigned int step, unsigned int aliveCount);
void drawNewFrame(unsigned short** arena, unsigned short sizeY, unsigned short sizeX,
                  unsigned int aliveCount, unsigned int tickCount, bool isAlive);

#ifdef CURSES_ALLOWED
void printTestFrame();
void drawLogo();
void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX);
#endif

#endif //ZAP_2023_8748_GRAPHIC_H
