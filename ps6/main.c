#include "./life/engine.h"
#include "./life/graphic.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    perror("enter map path as arg");
    return 1;
  }
  initCurses();
  drawTextLogo();
  sleep(1);
  play(argv[1]);
  deinitCurses();
  return 0;
}
