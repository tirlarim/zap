#include "superkarel.h"

void dropAllItems() {
  while (beepers_in_bag())
    put_beeper();
}

void takeAllItems() {
  while (beepers_present())
    pick_beeper();
}

void stepUntilWall() {
  while (front_is_clear()) { step(); }
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

void stepBack() {
  turnBack();
  step();
  turnBack();
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}

void prepare() {
  turnSouth();
  stepUntilWall();
  turnEast();
  stepUntilWall();
  turnNorth();
}

void fillWall() {
  while (true) {
    step();
    if (beepers_present()) {
      stepBack();
      break;
    }
    do {
      step();
    } while (front_is_clear() && !beepers_present());
    turnBack();
    if (beepers_present()) { step(); }
    put_beeper();
  }
}

void findCenter() {
  turnBack();
  while (front_is_clear()) {
    step();
    pick_beeper();
  }
  put_beeper();
  turnBack();
  step();
  while (!beepers_present())
    step();
  put_beeper();
  turn_left();
  while (front_is_clear())
    step();
  turnBack();
  step();
  while (front_is_clear() && beepers_present()) {
    pick_beeper();
    step();
  }
  turnBack();
  step();
  while (!beepers_present())
    step();
  takeAllItems();
  turnNorth();
}

void fillTopLeftCorner() {
  turnNorth();
  stepUntilWall();
  turnEast();
  stepUntilWall();
  turnBack();
  while (front_is_clear()) {
    step();
    put_beeper();
  }
  turn_left();
  while (front_is_clear()) {
    step();
    put_beeper();
  }
  pick_beeper();
}

void stupidRunner() {
  fillWall();
  turnWest();
  while (front_is_clear()) {
    step();
    if (!beepers_present() || front_is_clear())
      put_beeper();
  }
  //tmp
  turnEast();
  while (front_is_clear())
    step();
  turnRight();
  while (front_is_clear())
    step();
  takeAllItems();
  turnWest();
  fillWall();

  turn_left();
  while (front_is_clear()) {
    step();
    if (!beepers_present() || front_is_clear())
      put_beeper();
  }
  findCenter();
}


int main() {
  turn_on("task_5.kw");
  set_step_delay(20);
  fillTopLeftCorner();
  prepare();
  stupidRunner();
  turn_off();
  return 0;
}
