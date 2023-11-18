#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./hangman.h"

#define WORDLIST_FILENAME "words.txt"
#define WORD_LEN_MAX 30
#define TRY_COUNT_MAX 8
#define LETTERS_COUNT ('z'-'a'+1)

void lowercase(char* word) {
  unsigned long wordLen = strlen(word);
  for (int i = 0; i < wordLen; ++i) {
    if (word[i] >= 65 && word[i] <= 90)
      word[i]+=32;
  }
}

long getFileSize(FILE* fp) {
  long size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

int get_word(char* buffer) {
  long fileSize;
  FILE* fp = fopen(WORDLIST_FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
    return EXIT_FAILURE;
  }
  fileSize = getFileSize(fp);
  while (!*buffer) {
    fseek(fp, (rand() % fileSize) + 1, SEEK_SET);
    fscanf(fp, "%*s %s", buffer);
  }
  fclose(fp);
  return EXIT_SUCCESS;
}

int is_word_guessed(const char* secretWord, const char* lettersGuessed) {
  unsigned long secretWordLen = strlen(secretWord), lettersGuessedLen = strlen(lettersGuessed);
  for (int i = 0; i < secretWordLen; ++i) {
    bool findSymbol = false;
    for (int j = 0; j < lettersGuessedLen; ++j) {
      if (secretWord[i] == lettersGuessed[j]) {
        findSymbol = true;
        break;
      }
    }
    if (!findSymbol) return 0;
  }
  return 1;
}

void get_guessed_word(const char* secretWord, const char* lettersGuessed, char* wordGuessed) {
  unsigned long secretWordLen = strlen(secretWord), lettersGuessedLen = strlen(lettersGuessed);
  memset(wordGuessed, '_', secretWordLen*sizeof(char));
  wordGuessed[secretWordLen] = '\0';
  for (int i = 0; i < secretWordLen; ++i) {
    for (int j = 0; j < lettersGuessedLen; ++j) {
      if (secretWord[i] == lettersGuessed[j]) {
        wordGuessed[i] = secretWord[i];
        break;
      }
    }
  }
}

void getFormattedWord(const char* word, char* formattedWord) {
  unsigned long wordLen = strlen(word);
  for (int i = 0; i < wordLen; ++i) {
    formattedWord[i*2] = word[i];
  }
  formattedWord[wordLen*2-1] = '\0';
}

bool isLetterInWord(const char* secretWord, char letter) {
  unsigned long secretWordLen = strlen(secretWord);
  for (int i = 0; i < secretWordLen; ++i) {
    if (secretWord[i] == letter) return true;
  }
  return false;
}

bool isSymbolValid(char letter) {
  return letter <= 'z' && letter >= 'a' ? true : false;
}

bool isSymbolNew(const char* lettersGuessed, char letter) {
  unsigned long lettersGuessedLen = strlen(lettersGuessed);
  for (int i = 0; i < lettersGuessedLen; ++i) {
    if (lettersGuessed[i] == letter) return false;
  }
  return true;
}

void get_available_letters(const char* lettersGuessed, char* lettersAvailable) {
  unsigned long lettersGuessedLen = strlen(lettersGuessed), lettersAvailableIndex = 0;
  memset(lettersAvailable, '\0', (LETTERS_COUNT + 1) * sizeof(char));
  for (int i = 'a'; i <= 'z'; ++i) {
    bool findSymbol = false;
    for (int j = 0; j < lettersGuessedLen; ++j) {
      if (i == lettersGuessed[j]) {
        findSymbol = true;
        break;
      }
    }
    if (!findSymbol) lettersAvailable[lettersAvailableIndex++] = (char)i;
  }
}


void tickInput(char* input, const char* secretWord, char* currentWord, char* lettersGuessed, unsigned long inputLen,
               unsigned char* lettersGuessedIndex, unsigned int* tryCount, char* wordFormatted) {
  memset(input, '\0', inputLen);
  scanf("%s", input);
  lowercase(input);
  if (input[1] == '\0') { // input is symbol
    bool isLetterValid = isSymbolValid(input[0]);
    bool isLetterNew = isSymbolNew(lettersGuessed, input[0]);
    if (isLetterValid && isLetterNew) {
      lettersGuessed[*lettersGuessedIndex] = input[0];
      get_guessed_word(secretWord, lettersGuessed, currentWord);
      getFormattedWord(currentWord, wordFormatted);
      bool isLetterRight = isLetterInWord(secretWord, lettersGuessed[*lettersGuessedIndex]);
      --(*tryCount);
      ++(*lettersGuessedIndex);
      if (isLetterRight) {
        printf("Good guess: %s\n-------------\n", wordFormatted);
      } else {
        printf("Oops! That letter is not in my word: %s\n-------------\n", wordFormatted);
      }
    } else {
      if (!isLetterValid)
        printf("Oops! '%c' is not a valid letter: %s\n-------------\n", input[0], wordFormatted);
      if (!isLetterNew)
        printf("Oops! You've already guessed that letter: %s\n-------------\n", wordFormatted);
    }
    if (is_word_guessed(secretWord, lettersGuessed)) {
      *tryCount = 0;
      printf("Congratulations, you won!\n");
      return;
    }
    if (!*tryCount) {
      printf("Sorry, you ran out of guesses. The word was %s.\n", secretWord);
      return;
    }
  } else { // input is word
    if (!strcmp(secretWord, input)) {
      strcpy(currentWord, input);
      *tryCount = 0;
      printf("Congratulations, you won!\n");
      return;
    } else {
      *tryCount = 0;
      printf("Sorry, bad guess. The word was %s.", secretWord);
      return;
    }
  }
}

void hangman(const char* secretWord) {
  unsigned long secretWordLen = strlen(secretWord);
  unsigned char lettersGuessedIndex = 0;
  unsigned char currentWordSize = (secretWordLen+1)*sizeof(char);
  char* currentWord = (char*)malloc(currentWordSize);
  char* wordFormatted = (char*)malloc(currentWordSize*2);
  char* inputBuffer = (char*)malloc(WORD_LEN_MAX*sizeof(*inputBuffer));
  char availableLetters[LETTERS_COUNT + 1] = {0}, lettersGuessed[LETTERS_COUNT + 1] = {0};
  memset(currentWord, '_', currentWordSize-1);
  memset(wordFormatted, ' ', 2*currentWordSize-1);
  printf("Welcome to the game, Hangman!\n"
         "I am thinking of a word that is %ld letters long.\n"
         "-------------\n", secretWordLen);
  for (unsigned int tryCount = TRY_COUNT_MAX; tryCount && !is_word_guessed(secretWord, lettersGuessed);) {
    get_available_letters(lettersGuessed, availableLetters);
    printf("You have %u guesses left.\n"
           "Available letters: %s\n"
           "Please guess a letter: ", tryCount, availableLetters);
    tickInput(inputBuffer, secretWord, currentWord, lettersGuessed,
              WORD_LEN_MAX, &lettersGuessedIndex, &tryCount, wordFormatted);
  }
  free(currentWord);
  free(wordFormatted);
  free(inputBuffer);
}
