#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "graphic.h"
#include "../utils/utils.h"

typedef struct Terminal {
  struct Point2D size, availableSize, arenaSize, position, cursor, offset;
} TERMINAL;

struct winsize terminalSize;
struct Terminal terminalWindow;

void initCurses() {
  setlocale(LC_ALL, "");
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
    free(fileContent);
  }
  fclose(fp);
  refresh();
}

void drawTextLogo() {
  clear();
  FILE* fp = fopen(TEXT_LOGO_PATH, "rt");
  if (!fp) {
    if (has_colors()) attron(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
    printw("Unable to open logo file -> Path: %s", LOGO_PATH);
    if (has_colors()) attroff(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  } else {
    POINT_2D logoSize;
    fscanf(fp, "%hd %hd", &logoSize.x, &logoSize.y);
    char* fileContent = malloc(logoSize.x*logoSize.y* sizeof(char));
    unsigned int contentIndex = 0;
    do {
      fileContent[contentIndex] = getc(fp);
    } while (fileContent[contentIndex++] != EOF);
    unsigned short logoOffsetX =(terminalSize.ws_col-logoSize.x)/2;
    if (terminalSize.ws_row < logoSize.y) {
      for (int k = 0; k < logoSize.y - terminalSize.ws_row + 10; ++k) {
        clear();
        contentIndex = 0;
        for (int i = 0, j = 0; i <= logoSize.x; ++i) {
          if (fileContent[contentIndex] == '\n') {
            ++j;
            i = 0;
            contentIndex++;
            continue;
          }
          contentIndex++;
          mvprintw(-k+j, logoOffsetX+i, "%c", fileContent[contentIndex]);
        }
        refresh();
        milliSleep(50);
      }
    } else {
      unsigned short logoOffsetY =(terminalSize.ws_row-logoSize.y)/2;
      contentIndex = 0;
      for (int i = 0, j = 0; i <= logoSize.x; ++i) {
        char symbol = fileContent[contentIndex++];
        if (symbol == '\n') {
          ++j;
          i = 0;
          continue;
        }
        mvprintw(logoOffsetY+j, logoOffsetX+i, "%c", symbol);
      }
    }
    free(fileContent);
  }
  refresh();
}

void printArena(ARENA* arena, SETTINGS* settings, unsigned int step) {
  clear();
  attron(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  mvprintw(0, 0, "X: %d Y: %d, Frame: %u, alive: %u, fps: %u, Tick Per Frame: %u\n",
           terminalWindow.position.x, terminalWindow.position.y, step, arena->aliveCount, FPS, settings->tickPerFrame);
  attroff(COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  for (unsigned int i = 0; i < terminalWindow.availableSize.y; ++i) {
    for (unsigned int j = 0; j < terminalWindow.availableSize.x; ++j) {
      printw(arena->field[i + terminalWindow.position.y][j + terminalWindow.position.x] == 1 ? "x" : " ");
    }
  }
  if (settings->pause) {
    attron(COLOR_PAIR(YELLOW_ON_BLACK) | A_BOLD);
    mvprintw(terminalWindow.cursor.y + terminalWindow.offset.y, terminalWindow.cursor.x + terminalWindow.offset.x, "x");
    attroff(COLOR_PAIR(YELLOW_ON_BLACK) | A_BOLD);
  }
}

void drawNewFrame(ARENA* arena, SETTINGS* settings, unsigned int tickCount, bool isAlive) {
  flushinp();
  if (isAlive) {
    printArena(arena, settings, tickCount);
  } else printw("End life step %u\n", tickCount);
  refresh();
}

void updateWindowInfo(ARENA* arena) {
  unsigned short usedRows = 1;
  unsigned short usedColumns = 0;
  terminalWindow.offset.y = usedRows;
  terminalWindow.offset.x = usedColumns;
  terminalWindow.size.y = terminalSize.ws_row;
  terminalWindow.size.x = terminalSize.ws_col;
  terminalWindow.availableSize.y = terminalSize.ws_row - usedRows;
  terminalWindow.availableSize.x = terminalSize.ws_col - usedColumns;
  terminalWindow.arenaSize.y = arena->sizeY;
  terminalWindow.arenaSize.x = arena->sizeX;
  terminalWindow.cursor.x = 0;
  terminalWindow.cursor.y = 0;
}

void getTerminalSize(unsigned short* sizeY, unsigned short* sizeX) {
  *sizeY = terminalSize.ws_row;
  *sizeX = terminalSize.ws_col;
}

void graphicInputWorker(ARENA* arena, SETTINGS* settings, int key) {
  switch (key) {
    case '\n':
      settings->drawMode = !settings->drawMode;
      break;
    case KEY_MOVE_UP:
      if (terminalWindow.position.y)
        --terminalWindow.position.y;
      break;
    case KEY_MOVE_DOWN:
      if (terminalWindow.position.y + terminalWindow.availableSize.y < terminalWindow.arenaSize.y)
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
    case KEY_UP:
      terminalWindow.cursor.y = normalizeIndex(terminalWindow.cursor.y - 1, terminalWindow.availableSize.y);
      if (settings->drawMode) {
        POINT_2D pos;
        getCursorPos(&pos);
        arena->field[pos.y][pos.x] = 1;
      }
      break;
    case KEY_DOWN:
      terminalWindow.cursor.y = normalizeIndex(terminalWindow.cursor.y + 1, terminalWindow.availableSize.y);
      if (settings->drawMode) {
        POINT_2D pos;
        getCursorPos(&pos);
        arena->field[pos.y][pos.x] = 1;
      }
      break;
    case KEY_LEFT:
      terminalWindow.cursor.x = normalizeIndex(terminalWindow.cursor.x - 1, terminalWindow.availableSize.x);
      if (settings->drawMode) {
        POINT_2D pos;
        getCursorPos(&pos);
        arena->field[pos.y][pos.x] = 1;
      }
      break;
    case KEY_RIGHT:
      terminalWindow.cursor.x = normalizeIndex(terminalWindow.cursor.x + 1, terminalWindow.availableSize.x);
      if (settings->drawMode) {
        POINT_2D pos;
        getCursorPos(&pos);
        arena->field[pos.y][pos.x] = 1;
      }
      break;
    default:
      break;
  }
}

void getCursorPos(POINT_2D* pos) {
  pos->y = terminalWindow.cursor.y + terminalWindow.position.y;
  pos->x = terminalWindow.cursor.x + terminalWindow.position.x;
}
