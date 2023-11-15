#include <string.h>
#include "./utils.h"

void normalizeWord(char* word) {
  unsigned long wordLen = strlen(word);
  for (int i = 0; i < wordLen; ++i) {
    if (word[i] >= 65 && word[i] <= 90)
      word[i]+=32;
  }
}

void uppercase(char* word) {
  unsigned long wordLen = strlen(word);
  for (int i = 0; i < wordLen; ++i) {
    if (word[i] >= 97 && word[i] <= 122)
      word[i]-=32;
  }
}
