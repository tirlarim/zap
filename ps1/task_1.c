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
  loop(2) {
    turn_left();
  }
}

void turnRight() {
  loop(3) {
    turn_left();
  }
}

void cleanUp() {
  turnWest();
  takeAllItems();
}

void checkStart() {
  if (beepers_present()) {
    cleanUp();
  }
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}

void goVertical() { // start dir. = 0 (east)
  if (!beepers_in_bag() ? right_is_clear() : left_is_clear()) {
    !beepers_in_bag() ? turnRight() : turn_left();
  } else if (!beepers_in_bag() ? left_is_clear() : right_is_clear()) {
    !beepers_in_bag() ? turn_left() : turnRight();
  }

  while (facing_south() ? front_is_clear() : (!beepers_in_bag() ? right_is_blocked() : left_is_blocked()))
    stepAndCollect();

  if (!beepers_in_bag() ? right_is_clear() : left_is_clear()) { // climb
    !beepers_in_bag() ? turnRight() : turn_left();
    stepAndCollect();
  } else { // go down, stand on ground
    !beepers_in_bag() ? turn_left() : turnRight();
  }
}

void goHorizontal() { // start dir. = 0 (east)
  while (front_is_clear() && (!beepers_in_bag() ? right_is_blocked() : left_is_blocked())) {
    stepAndCollect();
  }
}

void wallRunnerLeft() {
  do { // left
    if (!beepers_in_bag() ? left_is_clear() : right_is_clear())
      !beepers_in_bag() ? turn_left() : turnRight();
    else if (!front_is_clear() && (!beepers_in_bag() ? right_is_clear() : left_is_clear()))
      !beepers_in_bag() ? turnRight() : turn_left();
    else if (!front_is_clear())
      turnBack();
    stepAndCollect();
  } while (!(beepers_present() && beepers_in_bag()));
}

void wallRunnerRight() {
  do { // right
    if (!beepers_in_bag() ? right_is_clear() : left_is_clear())
      !beepers_in_bag() ? turnRight() : turn_left();
    else if (!front_is_clear() && (!beepers_in_bag() ? left_is_clear() : right_is_clear()))
      !beepers_in_bag() ? turn_left() : turnRight();
    else if (!front_is_clear())
      turnBack();
    stepAndCollect();
  } while (!(beepers_present() && beepers_in_bag()));
}

void olympicRoad() {
  if (!front_is_clear()) {
    turnNorth();
  } else {
    turnEast();
  }
  dropAllItems();
  wallRunnerRight();
  wallRunnerLeft();
}

int main() {
  turn_on("task_1.kw");
  set_step_delay(20);
  if (!beepers_present()) {
    olympicRoad();
  }
  cleanUp();
  turn_off();
  return 0;
}
