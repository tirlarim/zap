#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "c4.h"

#define VERTICAL_BORDER '|'
#define BLANK '.'

// that does it mean? That this func should do? Fill all zero, dot or 1?
void initialize_board(int rows, int cols, char board[][cols]) {
  for (int i = 0; i < rows; ++i) {
    memset(board[i], BLANK, cols* sizeof(*board[i]));
  }
}

void print_board(int rows, int cols, const char board[][cols]) {
  for (int i = 0; i < rows; ++i) {
    printf("%c", VERTICAL_BORDER);
    for (int j = 0; j < cols; ++j) {
      printf(" %c |", board[i][j]);
    }
    printf("\n");
  }
  for (int i = 0; i < cols; ++i) {
    printf(" ---");
  }
  printf("\n");
  for (int i = 0; i < cols; ++i) {
    printf(" %2d ", i+1);
  }
  printf("\n");
}

int is_valid_move(int rows, int cols, const char board[][cols], int col) {
  return (--col >= 0 && col < cols && board[0][col] == BLANK);
}

int drop_piece(int rows, int cols, char board[rows][cols], int col, char player_piece) {
  if (!is_valid_move(rows, cols, board, col)) return -1;
  --col;
  for (int i = rows-1; i >= 0; --i) {
    if (board[i][col] == BLANK) {
      board[i][col] = player_piece;
      return i;
    }
  }
  return -1;
}


//check low-top
//check left-right
//check [low-left]-[top-right]
//check [top-left]-[low-right]
int check_win(int rows, int cols, const char board[][cols], int row, int col, char player_piece) {
  int indexes[2] = {0}, offsets[2] = {0};
  unsigned int combos[4] = {0};

  if (row == -1) for (int i = 0; i < rows && (board[i][col] == player_piece || board[i][col] == BLANK); ++i) row = i;

  for (int i = 0; row-i >= 0 && board[row-i][col] == player_piece; ++i) {indexes[0] = row-i;}
  for (int i = 0; col-i >= 0 && board[row][col-i] == player_piece; ++i) {indexes[1] = col-i;}
  for (int i = 0; row-i >= 0 && col-i >= 0 && board[row-i][col-i] == player_piece; ++i) {offsets[0] = i;}
  for (int i = 0; row+i < rows && col-i >= 0 && board[row+i][col-i] == player_piece; ++i) {offsets[1] = i;}


  for (int i = indexes[0]; i < rows && board[i][col] == player_piece; ++i) {++combos[0];}
  for (int i = indexes[1]; i < cols && board[row][i] == player_piece; ++i) {++combos[1];}
  for (int i = -offsets[0]; row+i >= 0 && col+i >= 0 && row+i < rows && col+i < cols && board[row+i][col+i] == player_piece; ++i) {++combos[2];}
  for (int i = offsets[1]; row+i >= 0 && col-i >= 0 && row+i < rows && col-i < cols && board[row+i][col-i] == player_piece; --i) {++combos[3];}

  for (int i = 0; i < 4; ++i) {
    if (combos[i] >= 4) {
      return 1;
    }
  }
  return 0;
}

int is_board_full(int rows, int cols, const char board[][cols]) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (board[i][j] == BLANK) return 0;
    }
  }
  return 1;
}

void play() {
  int rows = 4, cols = 5, colCurrent;
  bool isPlayer1Active = true;
  printf("Enter arena size: [x, y]\n");
  scanf("%d %d", &cols, &rows);
  char board[rows][cols]; // -Wno-vla-parameter-Wno-vla-parameter-Wno-vla-parameter-Wno-vla-parameter-Wno-vla-parameter
  initialize_board(rows, cols, board);
  print_board(rows, cols, board);
  while (!is_board_full(rows, cols, board)) {
    char currentItem = isPlayer1Active?'x':'o';
    printf("Player %d, enter your move (1-%d):", isPlayer1Active ? 1:2, cols);
    scanf("%d", &colCurrent);
    drop_piece(rows, cols, board, colCurrent, currentItem);
    print_board(rows, cols, board);
    if (check_win(rows, cols, board, -1, colCurrent, currentItem)) {
      printf("Win");
      break;
    }
    isPlayer1Active^=1;
  }
}
