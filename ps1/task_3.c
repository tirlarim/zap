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

void goToStartPos() {
  stepUntilWall();
  turnRight();
  stepUntilWall();
  turnRight();
  stepUntilWall();
  turnRight();
  stepUntilWall();
  turnRight();
}

void validateDirection() {
  turnEast();
}

void validateEndPosition() {
  turnEast();
  loop(3) {
    stepUntilWall();
    turnRight();
  }
  turnBack();
  step();
  validateDirection();
}

void verifyVertical() {
  turnNorth();
  loop(3) {
    stepUntilWall();
    turnRight();
  }
  while (true) {
    while (left_is_blocked()) { step(); }
    turn_left();
    step();
    while (!beepers_present() && front_is_clear()) { step(); }
    if (!front_is_clear()) {
      turnBack();
      stepUntilWall();
      turn_left();
      if (!front_is_clear()) { break; }
      step();
      turn_left();
      stepUntilWall();
      turnRight();
    } else {
      if (beepers_present()) {
        takeAllItems();
        turnRight();
        step();
        while (front_is_clear() && beepers_present()) {
          step();
        }
        dropAllItems();
        goToStartPos();
      }
    }
  }
}

void verifyHorizontal() {
  while (true) {
    while (front_is_clear()) {
      step();
      if (!beepers_in_bag() && beepers_present()) {
        pick_beeper();
        if (front_is_clear()) { step(); }
        else {
          dropAllItems();
          break;
        }
      }
      if (beepers_in_bag() && !beepers_present()) {
        put_beeper();
        turnBack();
        while (beepers_present()) {
          step();
        }
        turnBack();
      }
    }
    turnBack();
    while (beepers_present()) { step(); }
    if (beepers_in_bag()) { dropAllItems(); }
    stepUntilWall();
    turn_left();
    if (!front_is_clear()) { break; }
    step();
    turn_left();
  }
}


int main() {
  turn_on("task_3.kw");
  set_step_delay(20);
  validateDirection();
  verifyHorizontal();
  verifyVertical();
  validateEndPosition();
  turn_off();
  return 0;
}
