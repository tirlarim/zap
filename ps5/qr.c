#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// TODO: push after 3 Dec. 2023 12:00

#define BITS_PER_BYTE 8

typedef union bit_to_char {
  unsigned char character;
  struct bit {
    unsigned b0: 1; unsigned b1: 1;
    unsigned b2: 1; unsigned b3: 1;
    unsigned b4: 1; unsigned b5: 1;
    unsigned b6: 1; unsigned b7: 1;
  } mbit;
} BIT_TO_CHAR;

// Code example: void encode_string(const char string[], bool bytes[strlen(string)+1][8]) {...}
// Compiler ignore strlen(string)+1 or give error. You should not write cole like that.
// If 2D array is func param, you should write it as arr[][sizeX]
// If the compiler wants to get address in memory, where arr[2][3] stored it use simple formula:
// arr + (2 * sizeX + 3) * sizeof(dataType).
// SizeY is not important to us and will be ignored at compilation or will give an error.
// Interesting fact -> x86 architecture has special instruction to calc address in mem by this formula fast.

void encode_char(const char character, bool* bits) {
  unsigned char symbolSizeBits = sizeof(character) * BITS_PER_BYTE;
  for (unsigned char i = 0, j = symbolSizeBits - i - 1; i < symbolSizeBits; ++i, --j) bits[j] = character >> i & 1;
}

unsigned char decode_byte_old(const bool* bits) { // rewrite this ASAP
  unsigned char i = 0;
  BIT_TO_CHAR symbol;
  symbol.mbit.b7 = bits[i++];
  symbol.mbit.b6 = bits[i++];
  symbol.mbit.b5 = bits[i++];
  symbol.mbit.b4 = bits[i++];
  symbol.mbit.b3 = bits[i++];
  symbol.mbit.b2 = bits[i++];
  symbol.mbit.b1 = bits[i++];
  symbol.mbit.b0 = bits[i++];
  return symbol.character;
}

char decode_byte(const bool* bits) {
  unsigned char symbol = *bits;
  for (unsigned char i = 1; i < BITS_PER_BYTE; ++i) symbol = symbol << 1 | bits[i];
  return (char)symbol;
}

void encode_string(const char* string, bool bytes[][BITS_PER_BYTE]) {
  unsigned long inputLen = strlen(string);
  memset(bytes, 0, BITS_PER_BYTE*inputLen+BITS_PER_BYTE);
  for (unsigned long i = 0; i < inputLen; ++i) encode_char(*(string+i), *(bytes+i));
}

// Write bytes[][8] not bytes[rows][8]. Also use macros for const's is good idea, 8 is <magic value> and bad idea.
void decode_bytes(const int rows, bool bytes[][BITS_PER_BYTE], char* string) {
  for (unsigned int i = 0; i < rows; ++i) *(string+i) = decode_byte(*(bytes+i));
}

// :^) | ostav' svoi nadezhdy vsyak eto chitayushchiy.
void bytes_to_blocks(const int cols, const int offset, bool blocks[][cols], const int rows, bool bytes[][BITS_PER_BYTE]) {
  bool *bytesPtr = (bool*)bytes, *blockPtr = (bool*)blocks, *bytesMaxOffset = bytesPtr+rows*BITS_PER_BYTE;
  unsigned int blockSize = cols*BITS_PER_BYTE;
  for (int i = 0; i < offset; ++i)
    for (int j = 0; j < blockSize; ++j)
      *(blockPtr+(i*blockSize+(j*cols+j/BITS_PER_BYTE)%blockSize)) = bytesPtr < bytesMaxOffset ? *(bytesPtr++) : 0;
}

void blocks_to_bytes(const int cols, const int offset, bool blocks[][cols], const int rows, bool bytes[][BITS_PER_BYTE]) {
  bool *bytesPtr = (bool*)bytes, *blockPtr = (bool*)blocks;
  unsigned int blockSize = cols*BITS_PER_BYTE;
  for (int i = 0; i < offset; ++i)
    for (int j = 0; j < blockSize; ++j)
      *(bytesPtr++) = *(blockPtr+(i*blockSize+(j*cols+j/BITS_PER_BYTE)%blockSize));
}

#define TASK3

int main() {
#ifdef TASK1
  bool bits1[BITS_PER_BYTE] = {0};
  unsigned char symbol;
  for (char i = ' '; i != 127; ++i) {
    memset(bits1, '\0', BITS_PER_BYTE);
    encode_char(i, bits1);
    for (unsigned int j = 0; j < 8; j++) printf("%d", bits1[j]); printf(" -> ");
    symbol = decode_byte(bits1);
    printf("%3d -> <%c>\n", symbol, symbol);
  }
#endif
#ifdef TASK2
  char* text = "Hello, how are you?";
  const unsigned long len = strlen(text); // return size_t (unsigned long), not int
  bool bytes1[len][BITS_PER_BYTE]; // len+1? for what purpose? the first index is zero, not one
  encode_string(text, bytes1);
  for(unsigned long j = 0; j <= len; ++j){ // len is unsigned long
    printf("%c: ", text[j]);
    for(unsigned char i = 0; i < BITS_PER_BYTE; ++i){ // max int is 8 we can use char or unsigned char
      printf("%d", bytes1[j][i]);
    }
    printf("\n");
  }

  bool bytes2[7][8] = {
      {0,1,0,0,1,0,0,0},
      {0,1,1,0,0,1,0,1},
      {0,1,1,0,1,1,0,0},
      {0,1,1,0,1,1,0,0},
      {0,1,1,0,1,1,1,1},
      {0,0,1,0,0,0,0,1},
      {0,0,0,0,0,0,0,0}
  };
  char string[7];
  decode_bytes(7, bytes2, string);
  printf("%s\n", string);
#endif
#ifdef TASK3
#define BYTES1_SIZE_Y 5
#define BLOCK1_SIZE_Y (2*BITS_PER_BYTE)
#define BLOCK_SIZE_X 3
  int length = BYTES1_SIZE_Y, cols = BLOCK_SIZE_X, offset = BLOCK1_SIZE_Y/BITS_PER_BYTE;
  bool bytes1[BYTES1_SIZE_Y][BITS_PER_BYTE] = {
      {0,1,0,0,0,0,0,1},
      {0,1,1,0,1,0,0,0},
      {0,1,1,0,1,1,1,1},
      {0,1,1,0,1,0,1,0},
      {0,0,0,0,0,0,0,1},
  };
  bool blocks1[BLOCK1_SIZE_Y][BLOCK_SIZE_X];
  bytes_to_blocks(cols, offset, blocks1, length, bytes1);
  for(unsigned int i = 0; i < length; ++i){
    for(unsigned int j = 0; j < cols; ++j){
      printf("%d ", blocks1[i][j]);
    }
    printf("\n");
    if(!((i+1)&7)){
      printf("\n");
    }
  }

  bool blocks2[BLOCK1_SIZE_Y][BLOCK_SIZE_X] = {
      {0,0,0},
      {1,1,1},
      {0,1,1},
      {0,0,0},
      {0,1,1},
      {0,0,1},
      {0,0,1},
      {1,0,1},
      {0,0,0},
      {1,0,0},
      {1,0,0},
      {0,0,0},
      {1,0,0},
      {0,0,0},
      {1,0,0},
      {0,0,0}
  };
  bool bytes2[length][BITS_PER_BYTE];
  blocks_to_bytes(BLOCK_SIZE_X, BLOCK1_SIZE_Y/BITS_PER_BYTE, blocks2, length, bytes2);
  for(int j = 0; j < length; j++){
    for(int i = 0; i < BITS_PER_BYTE; i++){
      printf("%d", bytes2[j][i]);
    }
    printf("\n");
  }
#endif
}

