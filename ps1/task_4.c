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

void stepUntilWall() {
  while (front_is_clear()) { step(); }
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}

void repair() { //req. top position | at end top east
  while (!facing_north())
    turn_left();
  while (front_is_clear()) {
    if (!beepers_present()) {
      put_beeper();
    }
    step();
    if (!front_is_clear() && facing_north()) {
      turnBack();
    }
  }
}

void castleRepair() {
  while (true) {
    turn_left();
    while (!beepers_present() && front_is_clear()) {
      step();
    }
    if (beepers_present()) {
      turnBack();
      while (front_is_clear())
        step();
      turnBack();
      repair();
      turn_left();
      if (!front_is_clear()) {
        break;
      }
      step();
    }
    if (!front_is_clear() && facing_north()) {
      turnBack();
      while (front_is_clear())
        step();
      turn_left();
      if (!front_is_clear()) {
        break;
      }
      step();
    }
  }
}

void validateEnd() {
  turnEast();
  stepUntilWall();
}

int main() {
  turn_on("task_4.kw");
  set_step_delay(10);
  castleRepair();
  validateEnd();
  turn_off();
  return 0;
}
