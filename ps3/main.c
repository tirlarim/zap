#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hangman.h"
#include "morse.h"


int main() {
  char buff[30] = {0};
  time_t t;
  srand(time(&t));
  get_word(buff);
//  hangman(buff);
  char output[150];
  morse_to_text("--.-. ..- .-. ..-. ..- - ", output);
  puts(output);
  //prints: HELLO
  return 0;
}