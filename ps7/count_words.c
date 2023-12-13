#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#define FILENAME_INPUT "test.txt"
//#define FILENAME_OUTPUT "ans.txt"
#define WORD_LEN 32
#define SEARCH_WORD "ananas"
#define SEARCH_WORD_LEN 6

unsigned int normalizeIndex(int index, int limit) {
  return ((index % limit) + limit) % limit;
}

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

bool strCompare2(const char* str1, const char* str2, int endPont) {
  for (int i = 0; i < SEARCH_WORD_LEN; ++i) { // i = offset
    unsigned int index = normalizeIndex(endPont - SEARCH_WORD_LEN + i, SEARCH_WORD_LEN + 1);
//#ifdef DEBUG_FLAG
//    printf("%c[%d] - %c[%d]\n", str1[index], index, str2[i], i);
//#endif
    if (str1[index] != str2[i]) {
      return false;
    }
  }
//#ifdef DEBUG_FLAG
//  printf("OK\n");
//#endif
  return true;
}

void moveBack(char* str1) {
  for (int i = 1; i < SEARCH_WORD_LEN + 1; ++i) {
    str1[i - 1] = str1[i];
  }
}

int uintToString(unsigned int number, char* str) {
  int i = 0;
  for (; i < WORD_LEN && number > 0; ++i) {
    str[i] = (char) (number % 10 + '0');
    number /= 10;
  }
  return --i;
}

void task(char* pathInput, char* pathOutput) {
  FILE* fileInput = NULL;
  if (pathInput != NULL) fileInput = fopen(pathInput, "r");
  else return;
  const char wordMain[SEARCH_WORD_LEN] = SEARCH_WORD;
  char wordBf[SEARCH_WORD_LEN + 1] = {0}, ans[SEARCH_WORD_LEN + 1] = {0};
  unsigned int count = 0;
  if (fileInput == NULL) {
    fclose(fileInput);
    FILE* fileOutput = fopen(pathOutput, "w");
    fputc('0', fileOutput);
    fclose(fileOutput);
    return;
  }
// read first 7
  for (int i = 0; i <= SEARCH_WORD_LEN; ++i) {
    wordBf[i] = normalizeSymbol((char) fgetc(fileInput));
    if (wordBf[i] == EOF) {
      fclose(fileInput);
      FILE* fileOutput = fopen(pathOutput, "w");
      fputc('0', fileOutput);
      fclose(fileOutput);
      return;
    }
  }
  while (true) {
    if (strCompare2(wordBf, wordMain, SEARCH_WORD_LEN)) ++count;
    moveBack(wordBf);
    wordBf[6] = normalizeSymbol((char) fgetc(fileInput));
    if (wordBf[6] == EOF) break;
  }
  fclose(fileInput);
  FILE* fileOutput = fopen(pathOutput, "w");
  if (!count) fputc('0', fileOutput);
  else for (int i = uintToString(count, ans); i >= 0; --i) fputc(ans[i], fileOutput);
  fclose(fileOutput);
//#ifdef DEBUG_FLAG
//  printf("%d\n", count);
//#endif
}

int main(int argc, char** argv) {
  task(argv[1], argv[1]);
  return 0;
}
