#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    str[i] = (char) (number % 10 + '0');
    number /= 10;
  }
  return --i;
}

void task(char* path) {
  FILE* fileInput = fopen(path, "r");
  const char wordMain[WORD_LEN] = SEARCH_WORD;
  char wordBf[WORD_LEN] = {0}, ans[WORD_LEN] = {0};
  unsigned int inputWordLen = strLength(wordMain), count = 0, bufferIndex = 0;
  bool magicFlag = false;
  if (fileInput == NULL) {
    fclose(fileInput);
    perror("Error in opening file");
    return;
  }
  while (true) {
    wordBf[bufferIndex] = normalizeSymbol((char)fgetc(fileInput));
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
      for (int i = 0; i < WORD_LEN; ++i) wordBf[i] = 0;
      bufferIndex = 0;
    }
  }
  fclose(fileInput);
  FILE* fileOutput = fopen(path, "w");
  if (!count) fputc('0', fileOutput);
  else for (int i = uintToString(count, ans); i >= 0; --i) fputc(ans[i], fileOutput);
  fclose(fileOutput);
}

int main(int argc, char** argv) {
  task(argv[1]);
  return 0;
}
