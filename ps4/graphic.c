#include <stdlib.h>
#include <string.h>
#include "ballsortpuzzle.h"
#include "graphic.h"

#ifdef CURSES_ALLOWED
struct winsize terminalSize;

void initCurses() { // looks familiar?
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
  initscr();
//  getmaxyx(stdscr,row,col);
//  if (has_colors()) {
//    start_color();
//    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK); // p1
//    init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK); // p2
//    init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK); // default
//  }
//  noecho();
//  keypad(stdscr, TRUE);
//  nodelay(stdscr, TRUE);
//  cbreak();
//  curs_set(0);
}

void deinitCurses() {
  clear();
  refresh();
//  curs_set(1);
  endwin();
}

void drawTestFrame() {
  clear();
  for (int i = 0; i < terminalSize.ws_row; ++i) {
    for (int j = 0; j < terminalSize.ws_col; ++j) {
      mvprintw(i, j, "o");
    }
  }
  refresh();
}

void drawArena(ARENA* arena) {
  clear();
  for (int i = 0; i < arena->sizeY; ++i) {
    printw("%2d |", i + 1);
    for (int j = 0; j < arena->sizeX; ++j) {
      printw(" %c |", arena->data[i][j]);
    }
    printw("\n");
  }
  printw("   ");
  for (int i = 0; i < arena->sizeX; ++i) {
    printw(" ---");
  }
  printw("\n  ");
  for (int i = 0; i < arena->sizeX; ++i) {
    printw("  %2d", i + 1);
  }
  printw("\n");
  refresh();
}


void drawHappyEnd() {
  const char colorsUnix[7][8] = { // Have no idea is this work or not on win cmd/powershell
      ANSI_COLOR_RED,
      ANSI_COLOR_GREEN,
      ANSI_COLOR_YELLOW,
      ANSI_COLOR_BLUE,
      ANSI_COLOR_MAGENTA,
      ANSI_COLOR_CYAN,
      ANSI_COLOR_RESET,
  };
  for (unsigned int i = 0; i < 1024; ++i) {
    if (!(i & 7)) printf("\n");
    printw("%s Congratulations! %s You %s won! ", colorsUnix[rand() % 7], colorsUnix[rand() % 7],
           colorsUnix[rand() % 7]);
  }
}

void getUserInput(unsigned int* from, unsigned int* to) {
  printw("Enter column from: ");
  scanw("%u", from);
  printw("Enter column to: ");
  scanw("%u", to);
  refresh();
}

void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX) {
  *sizeY = terminalSize.ws_row;
  *sizeX = terminalSize.ws_col;
}

#else

void printArena(unsigned short **arena,
                unsigned short sizeY, unsigned short sizeX,
                unsigned int step, unsigned int aliveCount) {
  printf("Step: %d, alive: %u\n", step, aliveCount);
  for (unsigned short i = 0; i < sizeY; ++i) {
    for (unsigned short j = 0; j < sizeX; ++j) {
      printf(arena[i][j] ? "x" : " ");
    }
    printf("\n");
  }
}

void drawNewFrame(unsigned short** arena, unsigned short sizeY, unsigned short sizeX,
                  unsigned int aliveCount, unsigned int tickCount, bool isAlive) {
  if (isAlive) printArena(arena, sizeY, sizeX, tickCount, aliveCount);
  else printf("End life step %u\n", tickCount);
}

#endif
