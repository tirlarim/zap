#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME_INPUT "test.txt"
#define FILENAME_OUTPUT "ans.txt"
#define WORD_LEN 32
#define SEARCH_WORD "bananas"

unsigned int normalizeStr(char* string) {
  unsigned int offset = 0;
  bool detectLetter = false;
  for (int i = 0; i < WORD_LEN && string[i] != '\0'; ++i) {
    const bool isUppercase = string[i] >= 'A' && string[i] <= 'Z';
    const bool isLowercase = string[i] >= 'a' && string[i] <= 'z';
    if (!detectLetter && isUppercase || isLowercase) detectLetter = true;
    if (!detectLetter) ++offset;
    if (isUppercase) string[i] += 32;
  }
  return offset;
}

bool strCompare(char* str1, const char* str2) {
  const unsigned int startIndex = normalizeStr(str1);
  for (unsigned int i = startIndex, j = 0; str2[j] != '\0' && i < WORD_LEN; ++i, ++j) { // remove i < WORD_LEN
    if (str1[i] != str2[j]) {
      return false;
    }
  }
  return true;
}

void uintToString(unsigned int number, char* str) {
  for (unsigned int i = 0; i < WORD_LEN && number > 0; ++i) {
    str[i] = (char)((number % 10) + '0');
    number /= 10;
  }
}

int main() {
  FILE* fileInput = fopen(FILENAME_INPUT, "r");
  FILE* fileOutput = fopen(FILENAME_OUTPUT, "w");
  char wordMain[WORD_LEN] = SEARCH_WORD, wordBf[WORD_LEN] = {0}, ans[WORD_LEN] = {0};
  unsigned long count = 0;
  if (fileInput == NULL) {
    perror("Error in opening file");
    return 1;
  }
  for (unsigned int i = 0; true;) {
    wordBf[i] = (char)getc(fileInput);
    if (wordBf[i] == EOF) break;
    if (i >= WORD_LEN || wordBf[i] == ' ') {
      if (wordBf[i] == ' ') wordBf[i] = '\0';
      if (strCompare(wordBf, wordMain)) ++count;
      memset(wordBf, '\0', WORD_LEN);
      i = 0;
    } else ++i;
  }
  uintToString(count, ans);
  for (unsigned int i = 0; i < WORD_LEN && ans[i] != '\0'; ++i) putc(ans[i], fileOutput);
  printf("%lu\n", count);
  return 0;
}