#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "./hangman.h"
#include "./morse.h"

//hangman
#define WORDLIST_FILENAME "words.txt"
#define WORD_LEN_MAX 30
#define TRY_COUNT_MAX 8
#define LETTERS_COUNT ('z'-'a'+1)
//morse
#define MORSE_CODE_LEN 15000
#define MORSE_TEXT_LEN 15000
#define CODE_SIZE 7 // 5 for letters 6 for letters & numbers 7 for symbols
#define SYMBOLS_COUNT ('Z'-' '+1)

int main() {
#ifdef HANGMAN
  srand(time(NULL));
  char secretWord[WORD_LEN_MAX] = {0};
  if (get_word(secretWord)) {
    perror("Unable read words file");
    return EXIT_FAILURE;
  }
  unsigned char wordLen = strlen(secretWord);
  if (!wordLen) {
    perror("Unable to open file.\n");
    return EXIT_FAILURE;
  }
#ifdef DEBUG_FLAG
  printf("%s\n", secretWord);
#endif // DEBUG_FLAG
  hangman(secretWord);
#endif // HANGMAN
#ifdef MORSE
  unsigned long inputTextLen, inputMorseLen;
  char inputText[] = "yeah, this ^ is NTR!"; // -.-- . .- .... --..-- / - .... .. ... / .. ... / -. - .-. -.-.--
  char inputMorse[] = "-.-- . .- .... --..-- / - .... .. ... / .. ... / -. - .-. -.-.--"; // yeah, this is NTR!
  char* outputMorse;
  char* outputText;
  inputMorseLen = strlen(inputMorse), inputTextLen = strlen(inputText);
  outputText = (char*)calloc(inputMorseLen, sizeof(*inputMorse));
  outputMorse = (char*)calloc(inputTextLen*CODE_SIZE, sizeof(char));
  printf("Text to Morse >> %s (%lu)\n", inputText, inputTextLen);
  text_to_morse(inputText, outputMorse);
  printf("%s\n", outputMorse);
  printf("Morse to Text >> %s (%lu)\n", inputMorse, inputMorseLen);
  morse_to_text(inputMorse, outputText);
  printf("%s\n", outputText);
  free(outputMorse);
  free(outputText);
#endif // MORSE
  return EXIT_SUCCESS;
}
