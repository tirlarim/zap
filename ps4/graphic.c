#include <stdlib.h>
#include <string.h>
#include "ballsortpuzzle.h"
#include "graphic.h"

#ifdef CURSES_ALLOWED
void initColors();
struct winsize terminalSize;

void initCurses() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
  initscr();
  start_color();
  if (has_colors()) initColors();
}

void deinitCurses() {
  clear();
  refresh();
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
      printw(" ", arena->data[i][j]);
      if (arena->data[i][j] != ' ') attrset(COLOR_PAIR(arena->data[i][j]-'!'));
      printw("%c", arena->data[i][j]);
      if (arena->data[i][j] != ' ') attrset(COLOR_PAIR(0));
      printw(" |", arena->data[i][j]);
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
    printf("%s Congratulations! %s You %s won! ", colorsUnix[rand() % 7], colorsUnix[rand() % 7], colorsUnix[rand() % 7]);
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
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
  *sizeY = terminalSize.ws_row;
  *sizeX = terminalSize.ws_col;
}

void drawError() {
  printw("Unable to move, try different column\n");
}

void drawBoard(int rows, int cols, const char board[][cols]) {
  clear();
  for (int i = 0; i < rows; ++i) {
    printw("|");
    for (int j = 0; j < cols; ++j) {
      printw(" ");
      if (board[i][j] == 'x') attrset(COLOR_PAIR(62));
      if (board[i][j] == 'o') attrset(COLOR_PAIR(233));
      if (board[i][j] == ' ') attrset(COLOR_PAIR(1));
      printw("%c", board[i][j]);
      attrset(COLOR_PAIR(0));
      printw(" |");
    }
    printw("\n");
  }
  for (int i = 0; i < cols; ++i) {
    printw(" ---");
  }
  printw("\n");
  for (int i = 0; i < cols; ++i) {
    printw(" %2d ", i+1);
  }
  printw("\n");
  refresh();
}

void getInputC4(bool isPlayer1Active, int cols, int* colCurrent) {
  printw("Player %d, enter your move (1-%d):", isPlayer1Active ? 1:2, cols);
  scanw("%d", colCurrent);
  refresh();
}

void drawWinC4(bool isPlayer1Active) {
  char message1[] = "Player ";
  char message2[] = "Win Game ";
  unsigned long messageLen1 = strlen(message1);
  unsigned long messageLen2 = strlen(message2);
  int pair = 0;
  for (int b = 0; b < 16; b++)
    for (int f = 0; f < 16; f++, pair++)
      if (pair != 0)
        init_pair(pair, f, b);

  for (int i = 0; i < messageLen1; ++i) {
    attrset(COLOR_PAIR(rand()%16) | COLOR_BLACK);
    printw("%c", message1[i]);
  }
  printw("%d ", isPlayer1Active+1);
  for (int i = 0; i < messageLen2; ++i) {
    attrset(COLOR_PAIR(rand()%16) | COLOR_BLACK);
    printw("%c", message2[i]);
  }
//  printw("W");
//  attrset(COLOR_PAIR(233));
//  printw("i");
//  attrset(COLOR_PAIR(0));
//  printw("n\n");
  attrset(COLOR_PAIR(1));
  refresh();
  getch();
}

void initColors() {
  start_color();
  if (!has_colors() || !can_change_color() || (COLOR_PAIRS < 256) || (COLORS < 16)) return;
  init_color(0, 0, 0, 0);
  init_color(1, 0, 280, 730);
  init_color(2, 690, 750, 100);
  init_color(3, 490, 730, 910);
  init_color(4, 700, 520, 750);
  init_color(5, 450, 630, 760);
  init_color(6, 940, 970, 1000);
  init_color(7, 860, 180, 260);
  init_color(8, 770, 380, 60);
  init_color(9, 930, 850, 770);
  init_color(10, 620, 170, 410);
  init_color(11, 950, 610, 730);
  init_color(12, 670, 150, 310);
  init_color(13, 230, 480, 340);
  init_color(14, 1000, 750, 0);
  init_color(15, 600, 400, 800);
  init_color(16, 240, 860, 530);
  init_color(17, 780, 540, 400);
  init_color(18, 400, 360, 120);
  init_color(19, 570, 360, 510);
  init_color(20, 520, 110, 180);
  init_color(21, 980, 920, 840);
  init_color(22, 980, 810, 690);
  init_color(23, 0, 1000, 1000);
  init_color(24, 500, 1000, 830);
  init_color(25, 820, 1000, 80);
  init_color(26, 290, 440, 270);
  init_color(27, 910, 840, 420);
  init_color(28, 700, 750, 710);
  init_color(29, 1000, 600, 400);
  init_color(30, 990, 930, 0);
  init_color(31, 0, 500, 1000);
  init_color(32, 940, 1000, 1000);
  init_color(33, 540, 810, 940);
  init_color(34, 630, 790, 950);
  init_color(35, 960, 760, 760);
  init_color(36, 1000, 1000, 980);
  init_color(37, 1000, 570, 690);
  init_color(38, 980, 910, 710);
  init_color(39, 850, 90, 520);
  init_color(40, 490, 40, 10);
  init_color(41, 520, 520, 510);
  init_color(42, 740, 830, 900);
  init_color(43, 620, 510, 440);
  init_color(44, 960, 960, 860);
  init_color(45, 180, 350, 580);
  init_color(46, 610, 150, 260);
  init_color(47, 1000, 890, 770);
  init_color(48, 240, 170, 120);
  init_color(49, 590, 440, 90);
  init_color(50, 790, 880, 50);
  init_color(51, 0, 0, 0);
  init_color(52, 240, 50, 10);
  init_color(53, 330, 380, 440);
  init_color(54, 230, 240, 210);
  init_color(55, 750, 690, 700);
  init_color(56, 1000, 920, 800);
  init_color(57, 650, 440, 390);
  init_color(58, 190, 550, 910);
  init_color(59, 670, 900, 930);
  init_color(60, 400, 0, 0);
  init_color(61, 0, 0, 1000);
  init_color(62, 120, 460, 1000);
  init_color(63, 0, 580, 690);
  init_color(64, 0, 530, 740);
  init_color(65, 0, 90, 660);
  init_color(66, 200, 200, 600);
  init_color(67, 640, 640, 820);
  init_color(68, 400, 600, 800);
  init_color(69, 360, 680, 930);
  init_color(70, 70, 380, 500);
  init_color(71, 540, 170, 890);
  init_color(72, 310, 450, 650);
  init_color(73, 240, 410, 910);
  init_color(74, 870, 360, 510);
  init_color(75, 470, 270, 230);
  init_color(76, 890, 850, 790);
  init_color(77, 800, 250, 330);
  init_color(78, 850, 570, 940);
  init_color(79, 1000, 670, 110);
  init_color(80, 0, 260, 150);
  init_color(81, 800, 500, 200);
  init_color(82, 590, 290, 0);
  init_color(83, 690, 430, 300);
  init_color(84, 480, 710, 380);
  init_color(85, 1000, 780, 500);
  init_color(86, 500, 0, 130);
  init_color(87, 870, 720, 530);
  init_color(88, 630, 480, 450);
  init_color(89, 800, 330, 0);
  init_color(90, 910, 450, 320);
  init_color(91, 540, 200, 140);
  init_color(92, 740, 200, 640);
  init_color(93, 440, 160, 390);
  init_color(94, 370, 620, 630);
  init_color(95, 570, 640, 690);
  init_color(96, 0, 420, 240);
  init_color(97, 930, 530, 180);
  init_color(98, 650, 480, 360);
  init_color(99, 290, 210, 130);
  init_color(100, 640, 760, 680);
  init_color(101, 760, 600, 420);
  init_color(102, 940, 730, 800);
  init_color(103, 1000, 1000, 600);
  init_color(104, 1000, 940, 0);
  init_color(105, 890, 440, 480);
  init_color(106, 770, 120, 230);
  init_color(107, 0, 800, 600);
  init_color(108, 590, 0, 90);
  init_color(109, 840, 0, 250);
  init_color(110, 1000, 650, 790);
  init_color(111, 700, 110, 110);
  init_color(112, 340, 630, 830);
  init_color(113, 930, 570, 130);
  init_color(114, 440, 210, 260);
  init_color(115, 790, 350, 290);
  init_color(116, 670, 880, 690);
  init_color(117, 700, 1000, 1000);
  init_color(118, 870, 190, 390);
  init_color(119, 0, 480, 650);
  init_color(120, 160, 320, 750);
  init_color(121, 430, 610, 760);
  init_color(122, 110, 670, 840);
  init_color(123, 0, 250, 1000);
  init_color(124, 970, 910, 810);
  init_color(125, 950, 870, 810);
  init_color(126, 210, 270, 310);
  init_color(127, 900, 560, 670);
  init_color(128, 500, 1000, 0);
  init_color(129, 1000, 720, 770);
  init_color(130, 580, 270, 210);
  init_color(131, 890, 240, 160);
  init_color(132, 870, 440, 630);
  init_color(133, 670, 220, 120);
  init_color(134, 520, 380, 530);
  init_color(135, 1000, 700, 0);
  init_color(136, 480, 250, 0);
  init_color(137, 820, 410, 120);
  init_color(138, 600, 510, 480);
  init_color(139, 890, 260, 200);
  init_color(140, 800, 380, 490);
  init_color(141, 890, 820, 40);
  init_color(142, 620, 660, 120);
  init_color(143, 500, 90, 200);
  init_color(144, 440, 310, 220);
  init_color(145, 730, 850, 920);
  init_color(146, 970, 510, 470);
  init_color(147, 550, 570, 670);
  init_color(148, 720, 450, 200);
  init_color(149, 850, 540, 400);
  init_color(150, 680, 440, 410);
  init_color(151, 800, 430, 320);
  init_color(152, 600, 400, 400);
  init_color(153, 1000, 220, 0);
  init_color(154, 1000, 500, 310);
  init_color(155, 970, 510, 470);
  init_color(156, 540, 250, 270);
  init_color(157, 980, 930, 360);
  init_color(158, 390, 580, 930);
  init_color(159, 1000, 970, 860);
  init_color(160, 180, 180, 530);
  init_color(161, 1000, 970, 910);
  init_color(162, 510, 380, 240);
  init_color(163, 1000, 740, 850);
  init_color(164, 1000, 990, 820);
  init_color(165, 860, 80, 240);
  init_color(166, 620, 110, 200);
  init_color(167, 960, 960, 960);
  init_color(168, 0, 1000, 1000);
  init_color(169, 0, 720, 920);
  init_color(170, 350, 260, 490);
  init_color(171, 1000, 830, 0);
  init_color(172, 960, 440, 630);
  init_color(173, 1000, 850, 360);
  init_color(174, 400, 260, 130);
  init_color(175, 360, 220, 330);
  init_color(176, 0, 550, 550);
  init_color(177, 330, 410, 470);
  init_color(178, 720, 530, 40);
  init_color(179, 0, 390, 0);
  init_color(180, 100, 140, 130);
  init_color(181, 740, 720, 420);
  init_color(182, 280, 240, 200);
  init_color(183, 330, 240, 220);
  init_color(184, 550, 0, 550);
  init_color(185, 330, 420, 180);
  init_color(186, 1000, 550, 0);
  init_color(187, 600, 200, 800);
  init_color(188, 190, 100, 200);
  init_color(189, 550, 0, 0);
  init_color(190, 910, 590, 480);
  init_color(191, 560, 740, 560);
  init_color(192, 240, 80, 80);
  init_color(193, 550, 750, 840);
  init_color(194, 280, 240, 550);
  init_color(195, 180, 310, 310);
  init_color(196, 90, 450, 270);
  init_color(197, 0, 810, 820);
  init_color(198, 580, 0, 830);
  init_color(199, 330, 330, 330);
  init_color(200, 850, 200, 530);
  init_color(201, 980, 840, 650);
  init_color(202, 730, 310, 280);
  init_color(203, 0, 290, 290);
  init_color(204, 1000, 80, 580);
  init_color(205, 1000, 600, 200);
  init_color(206, 0, 750, 1000);
  init_color(207, 290, 390, 420);
  init_color(208, 490, 370, 380);
  init_color(209, 80, 380, 740);
  init_color(210, 130, 260, 710);
  init_color(211, 760, 600, 420);
  init_color(212, 930, 790, 690);
  init_color(213, 410, 410, 410);
  init_color(214, 120, 560, 1000);
  init_color(215, 290, 250, 160);
  init_color(216, 0, 0, 610);
  init_color(217, 940, 870, 730);
  init_color(218, 330, 360, 310);
  init_color(219, 760, 700, 500);
  init_color(220, 110, 110, 110);
  init_color(221, 380, 250, 320);
  init_color(222, 940, 920, 840);
  init_color(223, 800, 1000, 0);
  init_color(224, 750, 0, 1000);
  init_color(225, 560, 0, 1000);
  init_color(226, 310, 780, 470);
  init_color(227, 420, 190, 510);
  init_color(228, 710, 510, 580);
  init_color(229, 670, 290, 320);
  init_color(230, 800, 280, 290);
  init_color(231, 340, 240, 360);
  init_color(232, 0, 1000, 250);
  init_color(233, 590, 780, 640);
  init_color(234, 760, 600, 420);
  init_color(235, 500, 90, 90);
  init_color(236, 710, 200, 540);
  init_color(237, 870, 320, 520);
  init_color(238, 900, 670, 440);
  init_color(239, 310, 470, 260);
  init_color(240, 420, 330, 120);
  init_color(241, 1000, 330, 440);
  init_color(242, 410, 190, 410);
  init_color(243, 700, 130, 130);
  init_color(244, 810, 130, 160);
  init_color(245, 890, 350, 130);
  init_color(246, 930, 860, 510);
  init_color(247, 640, 0, 430);
  init_color(248, 1000, 980, 940);
  init_color(249, 130, 550, 130);
  init_color(250, 650, 480, 360);
  init_color(251, 520, 430, 300);
  init_color(252, 0, 450, 730);
  init_color(253, 990, 250, 570);
  init_color(254, 530, 380, 560);
  init_color(255, 620, 990, 220);
  init_color(256, 830, 450, 830);
  init_color(257, 990, 420, 620);
  init_color(258, 780, 170, 280);
  init_color(259, 470, 710, 1000);
  init_color(260, 530, 20, 810);
  init_color(261, 910, 210, 650);
  init_color(262, 1000, 0, 1000);
  init_color(263, 760, 330, 760);
  init_color(264, 890, 520, 0);
  init_color(265, 530, 260, 120);
  for (short i = 0, pair = 0; i < 265; i++, pair++) init_pair(pair, 7, i);
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
