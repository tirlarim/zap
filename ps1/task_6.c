#include "superkarel.h"

void dropAllItems() {
  while (beepers_in_bag())
    put_beeper();
}

void takeAllItems() {
  while (beepers_present())
    pick_beeper();
}

void turnEast() { while (!facing_east()) { turn_left(); }} // >
void turnNorth() { while (!facing_north()) { turn_left(); }} // ^
void turnWest() { while (!facing_west()) { turn_left(); }} // <
void turnSouth() { while (!facing_south()) { turn_left(); }} // v


void turnBack() {
  loop(2) { turn_left(); }
}

void turnRight() {
  loop(3) { turn_left(); }
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}

void validateEnd() {
  takeAllItems();
  turnEast();
}

void treasureHunter() {
  while (true) {
    while (!beepers_present()) { step(); }
    turnEast();
    loop(4) {
      if (beepers_present()) {
        pick_beeper();
        turn_left();
      }
    }
    if (beepers_present()) {
      break;
    }
  }
}

int main() {
  turn_on("task_6.kw");
  set_step_delay(20);
  treasureHunter();
  validateEnd();
  turn_off();
  return 0;
}
