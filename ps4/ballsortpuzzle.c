#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "ballsortpuzzle.h"

//TODO: disable for arena compilation
#ifdef CURSES_ALLOWED
#include "graphic.h"
#endif
#ifdef HAPPY_GAME_END
#include "printColors.h"
#endif

#define VOID_COLUMNS_COUNT 2
#define MIN_ARENA_SIZE_X 4
#define MIN_ARENA_SIZE_Y 2
#define FIRST_ITEM_SYMBOL '!'
#define LAST_ITEM_SYMBOL '~'
#define MAX_ARENA_SIZE_Y (LAST_ITEM_SYMBOL-FIRST_ITEM_SYMBOL)

void shuffle(unsigned char* array, unsigned int n) {
  if (n > 1) {
    unsigned int i;
    for (i = 0; i < n - 1; i++) {
      unsigned int j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

bool include(const unsigned int* array, unsigned int arrLen, unsigned char item) {
  for (int i = 0; i < arrLen; ++i) if (array[i] == item) return true;
  return false;
}

void fillArrayUnique(unsigned int* array, unsigned int arrLen, unsigned int min, unsigned int max) {
  for (int i = 0; i < arrLen;) {
    unsigned int currentUnique = (rand() % max - min) + min;
    if (!include(array, i, currentUnique)) array[i++] = currentUnique;
  }
}

#ifdef CUSTOM_ARENA_ALLOWED

bool check2DArray(ARENA* arena) {
  for (int i = 0; i < arena->sizeX; ++i) {
    unsigned char currentElement = arena->data[0][i];
    bool isColumnOk = false;
    if (currentElement == BLANK) continue;
    for (int j = 1; j < arena->sizeY; ++j) {
      if (arena->data[j][i] != currentElement) {
        isColumnOk = true;
        break;
      }
    }
    if (!isColumnOk) return false;
  }
  return true;
}

void initArena(ARENA* arena) {
  arena->data = malloc(arena->sizeY * sizeof(unsigned char*));
  for (int i = 0; i < arena->sizeY; ++i) {
    arena->data[i] = calloc(arena->sizeX, sizeof(unsigned char));
  }
}

void destroyArena(ARENA* arena) {
  for (int i = 0; i < arena->sizeY; ++i) {
    free(arena->data[i]);
  }
  free(arena->data);
}

void generator(ARENA* arena) {
  srand(time(NULL));
  bool isGenerationIncorrect = true;
  while (isGenerationIncorrect) {
    unsigned int filledColumnsCount = arena->sizeX - VOID_COLUMNS_COUNT, itemsCount =
        arena->sizeY * filledColumnsCount, currentItemIndex = 0;
    unsigned char* items = calloc(itemsCount, sizeof(*items));
    unsigned int* voidIndexes = calloc(VOID_COLUMNS_COUNT, sizeof(*voidIndexes));
    if (arena->sizeY > MAX_ARENA_SIZE_Y) {
      perror("Too many rows\n");
      return;
    }
    fillArrayUnique(voidIndexes, VOID_COLUMNS_COUNT, 0, arena->sizeX - 1);
    for (unsigned int i = 0; i < filledColumnsCount; ++i) {
      for (unsigned int j = 0; j < arena->sizeY; ++j) {
        items[currentItemIndex++] = FIRST_ITEM_SYMBOL + i;
      }
    }
    shuffle(items, itemsCount);
    currentItemIndex = 0;
    for (unsigned int i = 0; i < arena->sizeY; ++i) {
      for (unsigned int j = 0; j < arena->sizeX; ++j) {
        bool isVoidColumn = include(voidIndexes, VOID_COLUMNS_COUNT, j);
        arena->data[i][j] = !isVoidColumn ? items[currentItemIndex++] : BLANK;
      }
    }
    isGenerationIncorrect = !check2DArray(arena);
    if (isGenerationIncorrect) {
      for (int i = 0; i < arena->sizeY; ++i) {
        memset(arena->data[i], '\0', arena->sizeX);
      }
    }
    free(items);
    free(voidIndexes);
  }
}

void down_possible(ARENA* arena, unsigned int x, unsigned int y) {
  unsigned int sizeY = arena->sizeY-1, sizeX = arena->sizeX-1;
  unsigned char currentSymbol;
  unsigned char symbolDst = BLANK;
  --x; --y;
  if (x > sizeX || y > sizeX || x == y || arena->data[0][y] != BLANK || arena->data[sizeY][x] == BLANK) {
    drawError();
    return;
  }
  for (int i = 0; i < arena->sizeY; ++i) {
    if (arena->data[i][y] != BLANK) {
      symbolDst = arena->data[i][y];
      break;
    }
  }
  for (int i = 0; i < arena->sizeY; ++i) {
    if (arena->data[i][x] != BLANK) {
      currentSymbol = arena->data[i][x];
      if (symbolDst == BLANK || symbolDst == currentSymbol) {
        arena->data[i][x] = BLANK;
        break;
      } else {
        drawError();
        return;
      }
    }
  }
  for (int i = sizeY; i >= 0; --i) {
    if (arena->data[i][y] == BLANK) {
      arena->data[i][y] = currentSymbol;
      break;
    }
  }
}

bool check(ARENA* arena) {
  for (int i = 0; i < arena->sizeX; ++i) {
    bool isColumnValid = true;
    for (int j = 1; j < arena->sizeY; ++j) {
      if (arena->data[j][i] != arena->data[j - 1][i]) {
        isColumnValid = false;
        break;
      }
    }
    if (!isColumnValid) return false;
  }
  return true;
}

void game_field(ARENA* arena) {
#ifdef CURSES_ALLOWED
  drawArena(arena);
#else
  for (int i = 0; i < arena->sizeY; ++i) {
    printf("%2d |", i + 1);
    for (int j = 0; j < arena->sizeX; ++j) {
      printf(" %c |", arena->data[i][j]);
    }
    printf("\n");
  }
  printf("   ");
  for (int i = 0; i < arena->sizeX; ++i) {
    printf(" ---");
  }
  printf("\n");
  printf("  ");
  for (int i = 0; i < arena->sizeX; ++i) {
    printf("  %2d", i + 1);
  }
  printf("\n");
#endif
}

void gameTick(ARENA* arena) {
  unsigned int from, to;
#ifdef CURSES_ALLOWED
  getUserInput(&from, &to);
#else
  printf("Enter what: ");
  scanf("%u", &from);
  printf("Enter where: ");
  scanf("%u", &to);
  printf("\n");
#endif
  down_possible(arena, from, to);
}

void ball_sort_puzzle() {
  ARENA arena;
  char inputBfX[10] = {0}, inputBfY[10] = {0};
  unsigned short terminalSizeX = 0, terminalSizeY = 0;
  getTerminalSize(&terminalSizeY, &terminalSizeX);
  printf("Enter arena size [X Y]:\n");
  scanf("%s %s", inputBfX, inputBfY);
  for(unsigned char i = 0; inputBfX[i]; i++) inputBfX[i] = (char)tolower(inputBfX[i]);
  for(unsigned char i = 0; inputBfY[i]; i++) inputBfY[i] = (char)tolower(inputBfY[i]);
  if (strcmp(inputBfX, "auto") == 0) arena.sizeY = terminalSizeY-4;
  else arena.sizeY = strtoul(inputBfX, NULL, 10);
  if (strcmp(inputBfY, "auto") == 0) arena.sizeX = (terminalSizeX/4)-2;
  else arena.sizeX = strtoul(inputBfY, NULL, 10);
  if (arena.sizeY > MAX_ARENA_SIZE_Y || arena.sizeY < MIN_ARENA_SIZE_Y || arena.sizeY > terminalSizeY-4 ||
      arena.sizeX < MIN_ARENA_SIZE_X || arena.sizeX > (terminalSizeX/4)-2) {
    printf(ANSI_COLOR_RED":^)\n"ANSI_COLOR_RESET);
    exit(1);
  }
#ifdef CURSES_ALLOWED
  initCurses();
#endif
  initArena(&arena);
  generator(&arena);
  while (!check(&arena)) {
    game_field(&arena);
    gameTick(&arena);
  }
  game_field(&arena);
  destroyArena(&arena);
  // <...How the game ends, you can decide by yourself...>
  // I want to load Trojan as reward on PC someone lucky who win this game
  // But many <Congratulations!!!!!!> is also great idea.
#ifdef CURSES_ALLOWED
  deinitCurses();
#endif
#ifdef HAPPY_GAME_END
  drawHappyEnd();
#else
  printf("Congratulations! You won!\n");
#endif
}

#else
bool check2DArray(unsigned int rows, unsigned int columns, char array[][columns]) {
  for (int i = 0; i < columns; ++i) {
    unsigned char currentElement = array[0][i];
    bool isColumnOk = false;
    if (currentElement == BLANK) continue;
    for (int j = 1; j < rows; ++j) {
      if (array[j][i] != currentElement) {
        isColumnOk = true;
        break;
      }
    }
    if (!isColumnOk) return false;
  }
  return true;
}

void generator(const int rows, const int columns, char field[][columns]) {
//  srand(time(NULL));
  bool isGenerationIncorrect = true;
  while (isGenerationIncorrect) {
    unsigned int filledColumnsCount = columns - VOID_COLUMNS_COUNT, itemsCount = rows * filledColumnsCount;
    unsigned char* items = calloc(itemsCount, sizeof(*items)), currentItemIndex = 0;
    unsigned int* voidIndexes = calloc(VOID_COLUMNS_COUNT, sizeof(*voidIndexes));
    unsigned char firstSymbol = '!', lastSymbol = '~';
    if (rows > lastSymbol-firstSymbol) {
      perror("Too many rows\n");
      return;
    }
    fillArrayUnique(voidIndexes, VOID_COLUMNS_COUNT, 0, columns-1);
    for (unsigned int i = 0; i < filledColumnsCount; ++i) {
      for (unsigned int j = 0; j < rows; ++j) {
        items[currentItemIndex++] = firstSymbol+i;
      }
    }
    shuffle(items, itemsCount);
    currentItemIndex = 0;
    for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < columns; ++j) {
        bool isVoidColumn = include(voidIndexes, VOID_COLUMNS_COUNT, j);
        field[i][j] = !isVoidColumn ? items[currentItemIndex++] : BLANK;
      }
    }
    isGenerationIncorrect = !check2DArray(rows, columns, field);
    if (isGenerationIncorrect) {
      for (int i = 0; i < rows; ++i) {
        memset(field, '\0', columns);
      }
    }
    free(items);
    free(voidIndexes);
  }
}

void down_possible(const int rows, const int columns, char field[][columns], int x, int y) {
  unsigned int sizeY = rows-1, sizeX = columns-1;
  char currentSymbol;
  char symbolDst = BLANK;
  --x; --y;
  if (x < 0 || x > sizeX || y < 0 || y > sizeX || x == y || field[0][y] != BLANK || field[sizeY][x] == BLANK) {
    printf("Unable to move from column %d to %d -> try different column\n", x, y);
    return;
  }
  for (int i = 0; i < rows; ++i) {
    if (field[i][y] != BLANK) {
      symbolDst = field[i][y];
      break;
    }
  }
  for (int i = 0; i < rows; ++i) {
    if (field[i][x] != BLANK) {
      currentSymbol = field[i][x];
      if (symbolDst == BLANK || symbolDst == currentSymbol) {
        field[i][x] = BLANK;
        break;
      } else {
        printf("Unable to move from column %d to %d -> try different column\n", x, y);
        return;
      }
    }
  }
  for (int i = rows; i >= 0; --i) {
    if (field[i][y] == BLANK) {
      field[i][y] = currentSymbol;
      break;
    }
  }
}

bool check(const int rows, const int columns, char field[][columns]) {
  for (int i = 0; i < columns; ++i) {
    bool isColumnValid = true;
    for (int j = 1; j < rows; ++j) {
      if (field[j][i] != field[j-1][i]) {
        isColumnValid = false;
        break;
      }
    }
    if (!isColumnValid) return false;
  }
  return true;
}

void game_field(const int rows, const int columns, char field[][columns]) {
  for (int i = 0; i < rows; ++i) {
    printf("%2d |", i+1);
    for (int j = 0; j < columns; ++j) {
      printf(" %c |", field[i][j]);
    }
    printf("\n");
  }
  printf("   ");
  for (int i = 0; i < columns; ++i) {
    printf(" ---");
  }
  printf("\n");
  printf("  ");
  for (int i = 0; i < columns; ++i) {
    printf("  %2d", i+1);
  }
  printf("\n");
}


void ball_sort_puzzle() {
  int sizeY = 4, sizeX = 6;
  char arena[4][6] = {0};
  if (sizeY > MAX_ARENA_SIZE_Y || sizeX < MIN_ARENA_SIZE_X || sizeY < MIN_ARENA_SIZE_Y) {
    printf(":^)");
    exit(1);
  }
  generator(sizeY, sizeX, arena);
  while (!check(sizeY, sizeX,arena)) {
    game_field(sizeY, sizeX,arena);
    int from, to;
    printf("Enter what: ");
    scanf("%d", &from);
    printf("Enter where: ");
    scanf("%d", &to);
    printf("\n");
    down_possible(sizeY, sizeX, arena, from, to);
  }
  game_field(sizeY, sizeX, arena);
  printf("Congratulations! You won!\n");
}
#endif



