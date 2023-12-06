#include "./life/engine.h"
#include "./life/graphic.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    perror("enter map path as arg");
    return 1;
  }
  initCurses();
  play(argv[1]);
//  drawLogo();
//  getchar();
//  getchar(); // any ideas why if I write only one getchar(), it doesn't work?
  deinitCurses();
  return 0;
}
