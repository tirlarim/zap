#ifndef ZAP_2023_8748_GRAPHIC_H
#define ZAP_2023_8748_GRAPHIC_H


//#define CURSES_ALLOWED
#include <stdbool.h>
#ifdef CURSES_ALLOWED
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#else
#include <stdio.h>
#endif

#include "./ballsortpuzzle.h"
#include "printColors.h"


enum Colors {
  RED_ON_BLACK = 1,
  YELLOW_ON_BLACK = 2,
  WHITE_ON_BLACK = 3,
};

void initCurses();
void deinitCurses();
void drawArena(ARENA* arena);
void drawError();
void drawBoard(int rows, int cols, const char board[][cols]);
void drawHappyEnd();

#ifdef CURSES_ALLOWED
void printTestFrame();
void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX);
void getUserInput(unsigned int* from, unsigned int* to);
void getInputC4(bool isPlayer1Active, int cols, int* colCurrent);
void drawWinC4(bool isPlayer1Active);
#endif

#endif //ZAP_2023_8748_GRAPHIC_H
