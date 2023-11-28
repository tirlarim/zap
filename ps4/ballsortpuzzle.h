#ifndef SORT_PUZZLE
#define SORT_PUZZLE

#include <stdbool.h>

#define BLANK ' '

// disable for arena compilation
//#define CUSTOM_ARENA_ALLOWED
//#ifdef __APPLE__
//#define HAPPY_GAME_END
//#endif
//#ifdef __linux__
//#define HAPPY_GAME_END
//#endif

typedef struct Arena {
  unsigned int sizeX, sizeY;
  unsigned char** data;
}ARENA;

/**
 * Function draw game field
 * @param rows count of rows
 * @param columns count of columns
 * @param field hold 2d array of characters in game
 */
#ifdef CUSTOM_ARENA_ALLOWED
void game_field(ARENA* arena); // why here int ? rows can be only 0 or higher
#else
void game_field(const int rows, const int columns, char field[rows][columns]);
#endif


/**
 * Check fields rows if there are same characters
 * @param rows count of rows
 * @param columns count of columns
 * @param field hold 2d array of characters in game
 * @return true, if columns are complete; false otherwise
 */
#ifdef CUSTOM_ARENA_ALLOWED
bool check(ARENA* arena);
#else
bool check(const int rows, const int columns, char field[rows][columns]);
#endif



/**
 * Function returns 2D array after moving characters based on player input
 * When characters are not same, nothing happens and warning appears
 * Function also find the most upper character and the lower character from columns
 * @param rows count of rows
 * @param columns count of columns
 * @param field hold 2d array of characters in game
 * @param x is column from where character should be moved
 * @param y is column where character should be moved
 */
#ifdef CUSTOM_ARENA_ALLOWED
void down_possible(ARENA* arena, unsigned int xSrc, unsigned int xDst);
#else
void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y);
#endif


/**
 * Function returns 2D array of randomly generated characters
 * There must be two blank columns and other should be full of characters without '\0' or blank spaces
 * @param rows count of rows
 * @param columns count of columns
 * @param field hold 2d array of characters in game
 */
#ifdef CUSTOM_ARENA_ALLOWED
void generator(ARENA* arena);
#else
void generator(const int rows, const int columns, char field[rows][columns]);
#endif


/**
 * Starts interactive ball sort puzzle game
 */
void ball_sort_puzzle();

#endif //SORT_PUZZLE