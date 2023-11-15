#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "morse.h"

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
  memset(output, '\0', MORSE_CODE_LEN*sizeof(char));
  uppercase(formattedInput);
  for (int i = 0; i < inputLen; ++i) {
#ifdef DEBUG_FLAG
    printf("symbol: %c (%d)\n", formattedInput[i], formattedInput[i]);
    printf("code index: %d\n", formattedInput[i]-'A'+33);
    printf("code: %s\n", codes[formattedInput[i]-'A'+33]);
#endif
    strcat(output, codes[formattedInput[i]-'A'+33]);
    if (i+1 < inputLen) strcat(output, " ");
  }
  destroyMorseTable(codes);
  free(formattedInput);
}

void morse_to_text(const char* input, char* output) {
  unsigned long inputLen = strlen(input);
  unsigned long outputIndex = 0;
  char codeBuffer[CODE_SIZE] = {0}, codeBufferIndex = 0;
  char** codes = getMorseCodes();
  for (int i = 0; i < inputLen; ++i) {
    if (input[i] != ' ')
      codeBuffer[codeBufferIndex++] = input[i];
    if (input[i] == ' ' || i+1 == inputLen) {
      for (int j = 0; j < SYMBOLS_COUNT; ++j) {
        if (!strcmp(codes[j], codeBuffer)) {
#ifdef DEBUG_FLAG
          printf("code index: %d\n", j);
          printf("code: %s\n", codes[j]);
          printf("symbol: %c\n", j+' ');
#endif
          output[outputIndex++] = (char)(j+' ');
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
  char codeBuffer[CODE_SIZE] = {0}, codeBufferIndex = 0;
  char** codes = getMorseCodes();
  for (int i = 0; i < morseCodeLen; ++i) {
    if (morseCode[i] != ' ')
      codeBuffer[codeBufferIndex++] = morseCode[i];
    if (morseCode[i] == ' ' || i+1 == morseCodeLen) {
      bool findCode = false;
      for (int j = 0; j < SYMBOLS_COUNT; ++j) {
        if (!strcmp(codes[j], codeBuffer)) {
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
