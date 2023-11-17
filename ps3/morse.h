#ifndef MORSE_H
#define MORSE_H

/**
 * Function detects, if provided morse code is valid or no,
 * If code is valid, function returns 1. If not, returns 0.
 * @param morseCode the word encoded into morse code
 */
int is_morse_code_valid(const char* morseCode);

/**
 * Function returns a string encoded into morse code.
 * @param input plain text string
 * @param output buffer for encoded string
 */
void text_to_morse(const char* input, char* output);

/**
 * Function returns a string decoded into readable form (alphabet).
 * @param input morse code string
 * @param output buffer for decoded string
 */
void morse_to_text(const char* input, char* output);

#endif //MORSE_H
