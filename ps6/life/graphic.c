#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphic.h"
#include "../utils/utils.h"

typedef struct Terminal {
  struct Point2D size, availableSize, arenaSize, position;
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
  attron(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  mvprintw(0, 0, "X: %d Y: %d, Step: %u, alive: %u, fps: %u\n",
           terminalWindow.position.x, terminalWindow.position.y, step, arena->aliveCount, FPS);
  attroff(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  for (unsigned int i = 0; i < terminalWindow.availableSize.y; ++i) {
    for (unsigned int j = 0; j < terminalWindow.availableSize.x; ++j) {
      printw(arena->field[i+terminalWindow.position.y][j+terminalWindow.position.x] == 1 ? "x" : " ");
    }
  }
}

void drawNewFrame(ARENA* arena, unsigned int tickCount, bool isAlive) {
  flushinp();
  if (isAlive) printArena(arena, tickCount);
  else printw("End life step %u\n", tickCount);
  refresh();
}

void updateWindowInfo(ARENA* arena) {
  unsigned short usedRows = 1;
  unsigned short usedColumns = 0;
  terminalWindow.size.y = terminalSize.ws_row;
  terminalWindow.size.x = terminalSize.ws_col;
  terminalWindow.availableSize.y = terminalSize.ws_row-usedRows;
  terminalWindow.availableSize.x = terminalSize.ws_col-usedColumns;
  terminalWindow.arenaSize.y = arena->sizeY;
  terminalWindow.arenaSize.x = arena->sizeX;
}

void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX) {
  *sizeY = terminalSize.ws_row;
  *sizeX = terminalSize.ws_col;
}

bool inputWorker(int key) {
  switch (key) {
    case KEY_EXIT_GAME:
      return true;
    case KEY_MOVE_UP:
      if (terminalWindow.position.y)
        --terminalWindow.position.y;
      break;
    case KEY_MOVE_DOWN:
      if (terminalWindow.position.y + terminalWindow.availableSize.y <= terminalWindow.arenaSize.y)
        ++terminalWindow.position.y;
      break;
    case KEY_MOVE_LEFT:
      if (terminalWindow.position.x)
        --terminalWindow.position.x;
      break;
    case KEY_MOVE_RIGHT:
      if (terminalWindow.position.x + terminalWindow.availableSize.x <= terminalWindow.arenaSize.x)
        ++terminalWindow.position.x;
      break;
    default: break;
  }
  return false;
}
