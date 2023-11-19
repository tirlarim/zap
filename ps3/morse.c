#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "morse.h"

#define MORSE_CODE_LEN 15000
#define MORSE_TEXT_LEN 15000
#define CODE_SIZE 7 // 5 for letters 6 for letters & numbers 7 for symbols
#define SYMBOLS_COUNT ('Z'-' '+1)
#define FIRST_SYMBOL ' '

char** getMorseCodes() {
  unsigned char codeIndex = 0;
  char** codes = (char**)calloc(SYMBOLS_COUNT, sizeof(char*));
  for (int i = 0; i < SYMBOLS_COUNT; ++i)
    codes[i] = (char*)calloc(CODE_SIZE, sizeof(char));
  strcpy(codes[codeIndex++], "/");      strcpy(codes[codeIndex++], "-.-.--"); strcpy(codes[codeIndex++], ".-..-."); //   ! "
  strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], "");       // # $ %
  strcpy(codes[codeIndex++], ".-...");  strcpy(codes[codeIndex++], ".----."); strcpy(codes[codeIndex++], "-.--.");  // & ' (
  strcpy(codes[codeIndex++], "-.--.-"); strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], ".-.-.");  // ) * +
  strcpy(codes[codeIndex++], "--..--"); strcpy(codes[codeIndex++], "-....-"); strcpy(codes[codeIndex++], ".-.-.-"); // , - .
  strcpy(codes[codeIndex++], "-..-.");  strcpy(codes[codeIndex++], "-----");  strcpy(codes[codeIndex++], ".----");  // / 0 1
  strcpy(codes[codeIndex++], "..---");  strcpy(codes[codeIndex++], "...--");  strcpy(codes[codeIndex++], "....-");  // 2 3 4
  strcpy(codes[codeIndex++], ".....");  strcpy(codes[codeIndex++], "-....");  strcpy(codes[codeIndex++], "--...");  // 5 6 7
  strcpy(codes[codeIndex++], "---..");  strcpy(codes[codeIndex++], "----.");  strcpy(codes[codeIndex++], "---..."); // 8 9 :
  strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], "-...-");  // ; < =
  strcpy(codes[codeIndex++], "");       strcpy(codes[codeIndex++], "..--.."); strcpy(codes[codeIndex++], ".--.-."); // > ? @
  strcpy(codes[codeIndex++], ".-");     strcpy(codes[codeIndex++], "-...");   strcpy(codes[codeIndex++], "-.-.");   // A B C
  strcpy(codes[codeIndex++], "-..");    strcpy(codes[codeIndex++], ".");      strcpy(codes[codeIndex++], "..-.");   // D E F
  strcpy(codes[codeIndex++], "--.");    strcpy(codes[codeIndex++], "....");   strcpy(codes[codeIndex++], "..");     // G H I
  strcpy(codes[codeIndex++], ".---");   strcpy(codes[codeIndex++], "-.-");    strcpy(codes[codeIndex++], ".-..");   // J K L
  strcpy(codes[codeIndex++], "--");     strcpy(codes[codeIndex++], "-.");     strcpy(codes[codeIndex++], "---");    // M N O
  strcpy(codes[codeIndex++], ".--.");   strcpy(codes[codeIndex++], "--.-");   strcpy(codes[codeIndex++], ".-.");    // P Q R
  strcpy(codes[codeIndex++], "...");    strcpy(codes[codeIndex++], "-");      strcpy(codes[codeIndex++], "..-");    // S T U
  strcpy(codes[codeIndex++], "...-");   strcpy(codes[codeIndex++], ".--");    strcpy(codes[codeIndex++], "-..-");   // V W X
  strcpy(codes[codeIndex++], "-.--");   strcpy(codes[codeIndex++], "--..");                                         // Y Z
  return codes;
}

void destroyMorseTable(char** codes) {
  for (int i = 0; i < SYMBOLS_COUNT; ++i)
    free(codes[i]);
  free(codes);
}

void uppercase(char* word) {
  unsigned long wordLen = strlen(word);
  for (int i = 0; i < wordLen; ++i) {
    if (word[i] >= 97 && word[i] <= 122)
      word[i]-=32;
  }
}

void text_to_morse(const char* input, char* output) {
  unsigned long inputLen = strlen(input);
  char* formattedInput = (char*)malloc(inputLen*sizeof(*formattedInput));
  char** codes = getMorseCodes();
  strcpy(formattedInput, input);
  memset(output, '\0', inputLen*CODE_SIZE*sizeof(char));
  uppercase(formattedInput);
  for (int i = 0; i < inputLen; ++i) {
#ifdef DEBUG_FLAG
    printf("symbol: %c (%d)\n", formattedInput[i], formattedInput[i]);
    printf("code index: %d\n", formattedInput[i]-'A'+33);
    printf("code: %s\n", codes[formattedInput[i]-'A'+33]);
#endif
    int codeIndex = formattedInput[i]-'A'+33;
    unsigned long outputIndex = strlen(output);
    if (codeIndex < 0 || codeIndex >= SYMBOLS_COUNT) continue;
    if (codeIndex == 0 && (outputIndex < 2 || output[outputIndex - 2] == '/') != 0) continue;
    strcat(output, codes[formattedInput[i]-'A'+33]);
    if (i+1 < inputLen) strcat(output, " ");
  }
  destroyMorseTable(codes);
  free(formattedInput);
}

void morse_to_text(const char* input, char* output) {
  unsigned long inputLen = strlen(input);
  unsigned long outputIndex = 0;
  char codeBuffer[CODE_SIZE] = {0};
  unsigned int codeBufferIndex = 0;
  char** codes = getMorseCodes();
  memset(output, '\0', inputLen*sizeof(char));
  for (int i = 0; i < inputLen; ++i) {
    if (input[i] != FIRST_SYMBOL)
      codeBuffer[codeBufferIndex++] = input[i];
    if (input[i] == FIRST_SYMBOL || i+1 == inputLen) {
      for (int j = 0; j < SYMBOLS_COUNT; ++j) {
        if (!strcmp(codes[j], codeBuffer)) {
#ifdef DEBUG_FLAG
          printf("code index: %d\n", j);
          printf("code: %s\n", codes[j]);
          printf("symbol: %c\n", j+' ');
#endif
          if ((16 <= j && j <= 25) || j >= 33) output[outputIndex++] = (char)(j + ' ');
          else output[outputIndex++] = ' ';
          break;
        }
      }
      codeBufferIndex = 0;
      memset(codeBuffer, '\0', CODE_SIZE*sizeof(char));
    }
  }
  destroyMorseTable(codes);
}

int is_morse_code_valid(const char* morseCode) {
  unsigned long morseCodeLen = strlen(morseCode);
  char codeBuffer[CODE_SIZE] = {0};
  unsigned char codeBufferIndex = 0;
  char** codes = getMorseCodes();
  for (int i = 0; i < morseCodeLen; ++i) {
    if (codeBufferIndex > CODE_SIZE-1) return 0;
    if (morseCode[i] != ' ' && (morseCode[i] == '.' || morseCode[i] == '-')) codeBuffer[codeBufferIndex++] = morseCode[i];
    if (morseCode[i] == ' ' || i+1 == morseCodeLen) {
      bool findCode = false;
      for (int j = 16; j < SYMBOLS_COUNT; ++j) {
        if (!strcmp(codes[j], codeBuffer) && (j <= 25 || j >= 33)) {
#ifdef DEBUG_FLAG
          printf("code index: %d\n", j);
          printf("code: %s\n", codes[j]);
          printf("symbol: %c\n", j+' ');
#endif
          findCode = true;
          break;
        }
      }
      if (!findCode) {
        destroyMorseTable(codes);
        return 0;
      }
      codeBufferIndex = 0;
      memset(codeBuffer, '\0', CODE_SIZE*sizeof(char));
    }
  }
  destroyMorseTable(codes);
  return 1;
}
