#ifndef ZAP_2023_8748_GRAPHIC_H
#define ZAP_2023_8748_GRAPHIC_H

#include "engine.h"
#include <stdbool.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LOGO_PATH "./media/logo_79.txt"
#define TEXT_LOGO_PATH "./media/logo-ascii.txt"
#define FPS 30
#define KEY_MOVE_UP 'w'
#define KEY_MOVE_DOWN 's'
#define KEY_MOVE_LEFT 'a'
#define KEY_MOVE_RIGHT 'd'

typedef struct Point2D {
  unsigned short y, x;
}POINT_2D;

enum Colors {
  RED_ON_BLACK = 1,
  YELLOW_ON_BLACK,
  WHITE_ON_BLACK,
};

void initCurses();
void deinitCurses();
void drawNewFrame(ARENA* arena, SETTINGS* settings, unsigned int tickCount, bool isAlive);
void printTestFrame();
void drawLogo();
void drawTextLogo();
void updateWindowInfo(ARENA* arena);
void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX);
void graphicInputWorker(ARENA* arena, SETTINGS* settings, int key);
void getCursorPos(POINT_2D* pos);

#endif //ZAP_2023_8748_GRAPHIC_H
