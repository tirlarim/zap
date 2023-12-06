#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define FILENAME_INPUT "test.txt"
#define FILENAME_OUTPUT "ans.txt"
#define WORD_LEN 32
#define SEARCH_WORD "ananas"

char normalizeSymbol(const char symbol) {
  if (symbol >= 'A' && symbol <= 'Z') return symbol + 32;
  return symbol;
}

void normalizeStr(char* string) {
  for (int i = 0; i < WORD_LEN && string[i] != '\0'; ++i) string[i] = normalizeSymbol(string[i]);
}

bool strCompare(char* str1, const char* str2) {
  normalizeStr(str1);
  for (unsigned int i = 0; str2[i] != '\0'; ++i) if (str1[i] != str2[i]) return false;
  return true;
}

unsigned int strLength(const char* str) {
  for (unsigned int i = 0; true; ++i) if (str[i] == '\0') return i;
}

int uintToString(unsigned int number, char* str) {
  int i = 0;
  for (; i < WORD_LEN && number > 0; ++i) {
    str[i] = (char)(number % 10 + '0');
    number /= 10;
  }
  return --i;
}

void task() {
  FILE* fileInput = fopen(FILENAME_INPUT, "r");
  FILE* fileOutput = fopen(FILENAME_OUTPUT, "w");
  const char wordMain[WORD_LEN] = SEARCH_WORD;
  char wordBf[WORD_LEN] = {0}, ans[WORD_LEN] = {0};
  unsigned int inputWordLen = strLength(wordMain), count = 0, bufferIndex = 0;
  bool magicFlag = false;
  if (fileInput == NULL) {
    fclose(fileInput);
    fclose(fileOutput);
    perror("Error in opening file");
    exit(1);
  }
  while (true) {
    wordBf[bufferIndex] = normalizeSymbol((char)getc(fileInput));
    if (wordBf[bufferIndex] == EOF) break;
    if (wordBf[bufferIndex] == wordMain[bufferIndex]) {
      magicFlag = true;
      ++bufferIndex;
      if (bufferIndex == inputWordLen) {
        magicFlag = false;
        ++count;
      }
    } else magicFlag = false;
    if (!magicFlag) {
      memset(wordBf, '\0', bufferIndex); // this is a macros, not a function
      bufferIndex = 0;
    }
  }
  for (int i = uintToString(count, ans); i >= 0; --i) putc(ans[i], fileOutput);
  fclose(fileInput);
  fclose(fileOutput);
  printf("%u\n", count);
}

int main() {
  task();
  return 0;
}
