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
  turnEast();
  stepUntilWall();
  turnWest();
  stepUntilWall();
  turnNorth();
  while (!beepers_present()) step();
  turnEast();
  stepUntilWall();
  turnBack();
}

void validateDirection() {
  turnEast();
}

void markStartPosition() {
  turnSouth(); // v
  stepUntilWall();
  while (true) {
    turnEast(); // >
    while (front_is_clear() && !beepers_present()) {
      step();
    }
    if (beepers_present()) {
      pick_beeper();
      turnNorth();
      stepUntilWall();
      turnWest();
      stepUntilWall();
      put_beeper();
      break;
    }
    turnBack();
    stepUntilWall();
    turnNorth();
    step();
  }
  validateDirection();
}

void validateEndPosition() {
  takeAllItems();
  turnEast();
  stepUntilWall();
  turnBack();
  while (front_is_clear() && (!beepers_present())) step();
  turnSouth();
  while (front_is_clear() && beepers_present()) step();
  put_beeper();
  stepUntilWall();
  turnBack();
  stepUntilWall();
  turnWest();
  stepUntilWall();
  validateDirection();
}

void verifyVertical() {
  turnNorth();
  while (!beepers_present()) step();
  turnRight();
  stepUntilWall();
  turnWest(); // <|


  while (true) {
    while (front_is_clear() && (right_is_blocked() || !beepers_present())) {
      step();
    }
    if (!front_is_clear()) {
      break;
    }
    if (beepers_present() && !right_is_blocked()) {
      while (true) {
        if (beepers_present()) {
          pick_beeper();
        } else break;
        do {
          step();
        } while (front_is_clear() && beepers_present());
        if (beepers_in_bag()) put_beeper();
        turnBack();
        while (beepers_present()) step();
        turnRight();
        if (front_is_clear()) {
          step();
          turnWest();
        } else break;
      }
    }
    goToStartPos();
  }
}

void verifyHorizontal() {
  while (true) {
    while (front_is_clear()) {
      step();
      if (!beepers_in_bag() && beepers_present()) {
        pick_beeper();
        if (front_is_clear()) {step();}
        else {dropAllItems();break;}
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
    while (beepers_present()) {step();}
    if (beepers_in_bag()) {dropAllItems();}
    stepUntilWall();
    turn_left();
    if (!front_is_clear()) {break;}
    step();
    turn_left();
  }
}


int main() {
  turn_on("task_3.kw");
  set_step_delay(10);
  validateDirection();
  markStartPosition();
  validateDirection();
  verifyHorizontal();
  verifyVertical();
  validateEndPosition();
  turn_off();
  return 0;
}
