#include <stdlib.h>
#include <string.h>
#include "graphic.h"
#include "../utils/utils.h"

struct winsize terminalSize;

#ifdef CURSES_ALLOWED

void initCurses() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
  initscr();
  if (has_colors()) {
    start_color();
    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
  }
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  cbreak();
  curs_set(0);
}

void deinitCurses() {
  clear();
  refresh();
  curs_set(1);
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

void drawLogo() {
  clear();
  FILE* fp = fopen(LOGO_PATH, "r");
  if (!fp) {
    if (has_colors()) {
      attron(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
    }
    printw("Unable to open file -> Path: %s", LOGO_PATH);
    if (has_colors()) {
      attroff(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
    }
  } else {
    unsigned long fileSize = getFileSize(fp);
    char* fileContent = (char*)calloc((fileSize + 20), sizeof(*fileContent));
    fread(fileContent, sizeof(char), fileSize, fp);
    printw(fileContent);
  }
  fclose(fp);
  refresh();
}

void printArena(unsigned short** arena, unsigned short sizeY, unsigned short sizeX, unsigned int step,
                unsigned int aliveCount) {
  clear();
  mvprintw(0, 0, "Step: %d, alive: %u\n", step, aliveCount);
  for (unsigned short i = 0; i < terminalSize.ws_row-1; ++i) {
    for (unsigned short j = 0; j < terminalSize.ws_col-1; ++j) {
      printw(i < sizeY && j < sizeX ? arena[i][j] ? "x" : " " : " ");
    }
    printw("\n");
  }
}

void drawNewFrame(unsigned short** arena, unsigned short sizeY, unsigned short sizeX,
                  unsigned int aliveCount, unsigned int tickCount, bool isAlive) {
  if (isAlive) printArena(arena, sizeY, sizeX, tickCount, aliveCount);
  else printw("End life step %u\n", tickCount);
  refresh();
  milliSleep(1000 / FPS);
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
