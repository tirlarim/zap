#include <stdio.h>
#include <stdbool.h>

//#define FILENAME_INPUT "test.txt"
//#define FILENAME_OUTPUT "ans.txt"
#define WORD_LEN 32
#define START_WORD "START"
#define END_WORD "STOP"
#define SEPARATOR ' '

unsigned long getWordLen(const char* word) {
  unsigned long wordLen = 0;
  for (; word[wordLen] != '\0'; ++wordLen);
  return wordLen;
}

long getWordIndex(FILE* fp, const char* word) {
  long index = 0;
  char wordBf = 0;
  unsigned int bfIndex = 0;
  bool possibleWord = true;
  unsigned long wordLen = getWordLen(word);
  do {
    if (bfIndex == wordLen) return index - bfIndex;
    ++index;
    wordBf = (char)fgetc(fp);
    if (wordBf == SEPARATOR) {
      possibleWord = true;
      continue;
    }
    if (possibleWord && wordBf == word[bfIndex]) ++bfIndex;
    else {
      bfIndex = 0;
      possibleWord = false;
    }
  } while (wordBf != EOF);
  return -1;
}


void task(char* fileNameInput, char* filenameOutput) {
  FILE* fileInput = fopen(fileNameInput, "r");
  FILE* fileOutput = fopen(filenameOutput, "w");
  bool flag = false;
  bool spaceDelay = false;
  unsigned long startWordLen = getWordLen(START_WORD), fileIndex = 0;
  long startIndex = getWordIndex(fileInput, START_WORD),
  endIndex = getWordIndex(fileInput, END_WORD)+startIndex;
  fclose(fileInput);
  char word[WORD_LEN] = {0};
  unsigned int bfIndex = 0;
  fileInput = fopen(fileNameInput, "r");
  do {
    word[bfIndex] = (char)fgetc(fileInput); // -1 to hide space
    if (fileIndex > startIndex + startWordLen && fileIndex < endIndex + startWordLen -1) {
      if (flag) {
        if (word[bfIndex] != SEPARATOR) fputc(word[bfIndex], fileOutput);
        else spaceDelay = true;
      }
      if (word[bfIndex] == SEPARATOR) {
        flag = !flag;
        if (flag && spaceDelay) {
          fputc(SEPARATOR, fileOutput);
          spaceDelay = false;
        }
      }
    }
    ++fileIndex;
  } while (word[bfIndex] != EOF && fileIndex < endIndex + startWordLen -1);
}


int main(int argc, char** argv) {
  if (argc < 2) return 1;
  else if (argc == 2) task(argv[1], argv[1]);
  else if (argc == 3) task(argv[1], argv[2]);
  return 0;
}
