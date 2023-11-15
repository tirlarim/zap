#ifndef HANGMAN_H
#define HANGMAN_H

#define WORDLIST_FILENAME "words.txt"
#define WORD_LEN_MAX 30
#define TRY_COUNT_MAX 8
#define LETTERS_COUNT ('z'-'a'+1)

/**
 * Function detects, whether player guessed the secret word
 * Based on the letters player used for guessing, this function detects,
 * if it is possible to construct (and guess) the secret word from this set.
 * If it is possible, function returns 1. If not, returns 0.
 * @param secretWord the secret word lowercase
 * @param lettersGuessed the lowercase letters player already used in his guessing
 * @return 1, if word is guess; 0 otherwise.
 */
unsigned char is_word_guessed(const char* secretWord, const char* lettersGuessed);


/**
 * Function returns string representing the guessed word
 * This string contains visible letters, which were guessed successfully
 * by player at their correct positions. If there are still some hidden
 * letters, the character '_' will be used for their representation.
 * @param secretWord the secret word lowercase
 * @param lettersGuessed the lowercase letters player already used in his guessing
 * @param wordGuessed the constructed string as result of this function (output parameter)
 */
void get_guessed_word(const char* secretWord, const char* lettersGuessed, char* wordGuessed);


/**
 * Returns all letters, which were not used for guessing
 * Function returns set of lowercase letters sorted in alphabetical order. This
 * set will contain all letters from the alphabet, but it omit those, which
 * were already guessed.
 * @param lettersGuessed the lowercase letters player already used in his guessing
 * @param lettersAvailable set of lowercase not yet used letters
 */
void get_available_letters(const char* lettersGuessed, char* lettersAvailable);


/**
 * Starts interactive hangman game
 * @param secretWord the secret word lowercase
 */
void hangman(const char* secretWord);


/**
 * Function opens the file with words and read random one into the buffer.
 * If error 0 is returned, else word length is returned.
 * @param buffer buffer, where random word will be read
 * @return word length
 */
unsigned long get_word(char* buffer);

#endif //HANGMAN_H
