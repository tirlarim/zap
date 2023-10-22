#include "superkarel.h"

void turnEast() { while (!facing_east()) { turn_left(); }} // >
void turnNorth() { while (!facing_north()) { turn_left(); }} // ^
void turnWest() { while (!facing_west()) { turn_left(); }} // <
void turnSouth() { while (!facing_south()) { turn_left(); }} // v

void dropAllItems() {
  while (beepers_in_bag())
    put_beeper();
}

void takeAllItems() {
  while (beepers_present())
    pick_beeper();
}

void turnBack() {
  loop(2) {
    turn_left();
  }
}

void turnRight() {
  loop(3) {
    turn_left();
  }
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}


void validateEnd() {
  if (!beepers_in_bag()) {pick_beeper();}
  turnNorth();
}

void tunnelRunner() {
  dropAllItems();
  while (true) {
    while (front_is_clear()) {
      step();
      if (beepers_present() && !beepers_in_bag()) {
        takeAllItems();
        turnBack();
      }
    }
    if (left_is_clear()) {
      turn_left();
    } else if (right_is_clear()) {
      turnRight();
    } else if (!front_is_clear()) {
      break;
    }
  }
}


int main() {
  turn_on("task_2.kw");
  set_step_delay(200);
  if (!beepers_present()) {
    tunnelRunner();
  }
  validateEnd();
  turn_off();
  return 0;
}
