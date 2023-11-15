#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "./hangman.h"
#include "./morse.h"


int main() {
#ifdef HANGMAN
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
#endif // DEBUG_FLAG
  hangman(secretWord);
#endif // HANGMAN
#ifdef MORSE
  unsigned long inputLen;
  char input[] = "-.-- . .- .... --..-- / - .... .. ... / .. ... / -. - .-. -.-.--"; // yeah, this is NTR!
  char* output;
  inputLen = strlen(input);
//  scanf("%lu", &inputLen);
//  input = (char*)calloc(inputLen, sizeof(*input));
  output = (char*)calloc(inputLen, sizeof(*input));
//  scanf("%s", input);
//  printf("text to Morse >> %s (%lu)\n", input, inputLen);
//  text_to_morse(input, output);
//  printf("%s\n", output);
  printf("Morse to Text >> %s (%lu)\n", input, inputLen);
  morse_to_text(input, output);
  printf("%s\n", output);
  if (is_morse_code_valid(".... . .-.. .-.. ---")) { //prints: Code is valid!
    printf("Code is valid! \n");
  } else {
    printf("Code is invalid! \n");
  }

  if (is_morse_code_valid(".... . .-.--. .-.. ---")) { //prints: Code is invalid!
    printf("Code is valid! \n");
  } else {
    printf("Code is invalid! \n");
  }
#endif // MORSE
  return EXIT_SUCCESS;
}
