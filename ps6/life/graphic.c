#include <stdlib.h>
#include <string.h>
#include "graphic.h"
#include "../utils/utils.h"

typedef struct Terminal {
  unsigned short sizeY, sizeX, availableSizeX, availableSizeY;
}TERMINAL;

struct winsize terminalSize;
struct Terminal terminalWindow;

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

void printArena(ARENA* arena, unsigned int step) {
  clear();
  mvprintw(0, 0, "Step: %d, alive: %u\n", step, arena->aliveCount);
  for (unsigned short i = 0; i < terminalWindow.availableSizeY; ++i) {
    for (unsigned short j = 0; j < terminalWindow.availableSizeX; ++j) {
      printw(i < arena->sizeY && j < arena->sizeX ? arena->field[i][j] == 1 ? "x" : " " : " ");
    }
  }
}

void drawNewFrame(ARENA* arena, unsigned int tickCount, bool isAlive) {
  if (isAlive) printArena(arena, tickCount);
  else printw("End life step %u\n", tickCount);
  refresh();
  milliSleep(1000 / FPS);
}

void updateWindowInfo() {
  unsigned short usedRows = 1;
  unsigned short usedColumns = 0;
  terminalWindow.sizeY = terminalSize.ws_row;
  terminalWindow.sizeX = terminalSize.ws_col;
  terminalWindow.availableSizeY = terminalSize.ws_row-usedRows;
  terminalWindow.availableSizeX = terminalSize.ws_col-usedColumns;
}

void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX) {
  updateWindowInfo();
  *sizeY = terminalWindow.availableSizeY;
  *sizeX = terminalWindow.availableSizeX;
}
