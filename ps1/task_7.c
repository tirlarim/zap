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
  while (front_is_clear()) {step();}
}

void stepAndCollect() {
  step();
  if (beepers_present() && !beepers_in_bag())
    turnBack();
  while (beepers_present() && !beepers_in_bag())
    pick_beeper();
}

void cleanUp() {
  turnEast();
  loop(2) {
    while (front_is_clear()) {
      takeAllItems();
      step();
    }
    turnBack();
  }
  turnEast();
  stepUntilWall();
}

void fillRoad() {
  turnEast();
  while (true) {
    if (!beepers_present())
      put_beeper();
    if (!front_is_clear())
      break;
    step();
  }
  turnBack();
  while (front_is_clear())
    step();
  turnBack();
}

void goToStart() {
  turnEast();
  while (beepers_present())
    step();
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

void checkEntrances() { // req dir. = 0 | out dir. = 0
  takeAllItems();
  turn_left();
  step();
  if (beepers_present()) {
    turnBack();
    step();
    return;
  }
  loop(2) { put_beeper(); }
  wallRunnerRight();
  turnSouth();
  if (front_is_clear()) {
    while (beepers_present()) {
      pick_beeper();
      turn_left();
    }
    if (facing_east()) { // 1 item on ground
      put_beeper();
      turnNorth();
      wallRunnerLeft();
      if (beepers_present()) {
        takeAllItems();
        put_beeper();
      }
      turnSouth();
      step();
    } else if (facing_north()) { // 2 items on ground
      turnSouth();
      step();
    }
  } else {
    turnBack();
    step();
    put_beeper();
    wallRunnerLeft();
    pick_beeper();
    turnSouth();
    step();
  }
}

void roomChecker() {
  fillRoad();
  while (front_is_clear() && (!left_is_clear() || beepers_present())) {
    while (front_is_clear() && (!left_is_clear() || beepers_present())) {
      if (beepers_present() && left_is_clear()) {
        checkEntrances();
        goToStart();
      }
      if (front_is_clear()) {
        step();
      }
    }

    turnBack();
    while (!(beepers_present() && right_is_clear())) {
      if (!beepers_present() && !front_is_clear())
        return;
      step();
    }
    turnBack();
  }
}

int main() {
  turn_on("task_7.kw");
  set_step_delay(20);
  roomChecker();
  cleanUp();
  turn_off();
  return 0;
}
