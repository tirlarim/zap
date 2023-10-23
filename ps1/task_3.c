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
  turnEast();
  stepUntilWall();
  turnNorth();
  while (!beepers_present()) step();
  while (beepers_present() && front_is_clear()) step();
  if (!beepers_present()) {
    turnBack(); step();
  }
  turnWest();
  while (beepers_present() && front_is_clear()) step();
  if (!beepers_present()) {
    turnBack(); step(); pick_beeper(); turnWest();
  }
  stepUntilWall();
  dropAllItems();
  validateDirection();
}

void validateEndPosition() {
  takeAllItems();
  turnEast();
  while (!beepers_present()) step();
  turnBack(); step();
  dropAllItems();
  stepUntilWall();
  validateDirection();
}

void checkEnd() {
  while (!beepers_present()) step();
  if (!left_is_blocked()) {
    while (true) {
      turnWest();
      if (beepers_present()) {
        pick_beeper();
      } else {
        turnNorth();
        stepUntilWall();
        turn_left(); step(); turn_left();
        while (!beepers_present() && front_is_clear()) step();
        if (!front_is_clear()) break;
        if (beepers_present()) continue;
      }
      do {
        step();
      } while (front_is_clear() && beepers_present());
      if (beepers_in_bag()) put_beeper();
      turnBack();
      while (beepers_present()) step();
      turnRight();
      if (front_is_clear()) step();
      else break;
    }
    turnEast();
    step();
    turnSouth();
    step();
  }
  turnWest();
  stepUntilWall();
  turnEast();
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

// ./arenaMaps/3-6.kw
// task_3.kw
int main() {
  turn_on("task_3.kw");
  set_step_delay(10);
  validateDirection();
  verifyHorizontal();
  validateDirection();
  markStartPosition();
  verifyVertical();
  validateEndPosition();
  checkEnd();
  turn_off();
  return 0;
}
