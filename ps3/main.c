#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "./hangman.h"



int main() {
  srand(time(NULL));
  char secretWord[WORD_LEN_MAX] = {0},
  guessedWordAnswer[WORD_LEN_MAX] = {0}, availableLettersAnswer[LETTERS_COUNT+1] = {0};
  unsigned char wordLen = get_word(secretWord);
  if (!wordLen) {
    perror("Unable to open file.\n");
    return EXIT_FAILURE;
  }
#ifdef DEBUG_FLAG
  printf("%s\n", secretWord);
  printf("%d\n", is_word_guessed("hello", "eaeihoul"));
  get_guessed_word("container", "arpstxgoieyu", guessedWordAnswer);
  get_available_letters("arpstxgoieyu", availableLettersAnswer);
  printf("%s\n", guessedWordAnswer);
  printf("%s\n", availableLettersAnswer);
#endif
  hangman(secretWord);
  return EXIT_SUCCESS;
}
