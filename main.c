#include <stdio.h>
#include <math.h>

float roundTo(float n, unsigned char precision) {
  float a = powf(10, precision);
  return roundf(a * n) / a;
}

float lift_a_car(int leverLen, int humanWeight, int carWeight) {
  return roundTo(((float)humanWeight * leverLen) / (float)(carWeight + humanWeight), 2);
}

float unit_price(float price, unsigned int rollsCount, unsigned int piecesCount) {
  return roundTo(price / (float)(rollsCount * piecesCount) * 100, 2);
}

#define BANKNOTES_TYPES_COUNT 5

int bank_notes(int price) {
  if ((price < 0) || (price % 10)) return -1;
  unsigned int banknotesTypes[BANKNOTES_TYPES_COUNT] = {10, 20, 50, 100, 200};
  unsigned int ans = 0;
  for (int i = BANKNOTES_TYPES_COUNT; i >= 0; --i) {
    while (price >= banknotesTypes[i]) {
      price -= (int)banknotesTypes[i];
      ans += 1;
    }
  }
  return (int)ans;
}

unsigned int euler(int n) {
  int result = n;
  for (int i = 2; i * i <= n; ++i)
    if (n % i == 0) {
      while (n % i == 0)
        n /= i;
      result -= result / i;
    }
  if (n > 1)
    result -= result / n;
  return result;
}

void sortArray(int *array, unsigned int n) {
  char isSorted = 1;
  for (int i = 1; i < n; ++i) {
    int buffer;
    if (array[i] < array[i - 1]) {
      isSorted = 0;
      buffer = array[i - 1];
      array[i - 1] = array[i];
      array[i] = buffer;
    }
    if (i == n - 1 && !isSorted) {
      i = 1;
      isSorted = 1;
    }
  }
}

int find_missing_number(int arrSize, int *array) {
  int max = array[0];
  for (int i = 1; i < arrSize; ++i) {
    if (array[i] > max) max = array[i];
  }
  for (int i = 0; i < arrSize; ++i) {
    unsigned char flag = 1;
    for (int j = 0; j < arrSize; ++j) {
      if (array[j] == array[i]+1) {
        flag = 0;
        break;
      }
    }
    if (flag) {
      if (array[i] != max) {return array[i]+1;}
    }
  }
  return -1;
}

#define PASCAL_TRIANGLE_MAX_SIZE 100

void generatePascalTriangle(unsigned int n, unsigned long triangle[PASCAL_TRIANGLE_MAX_SIZE][PASCAL_TRIANGLE_MAX_SIZE]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      if (n == j || j == 0) {
        triangle[i][j] = 1;
      } else {
        triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
      }
    }
  }
}

void printPascalTriangle(unsigned int n, unsigned long triangle[PASCAL_TRIANGLE_MAX_SIZE][PASCAL_TRIANGLE_MAX_SIZE]) {
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < n; ++j) {
      if (triangle[i][j]) {
        printf("%3lu ", triangle[i][j]);
      }
    }
    printf("\n");
  }
}

unsigned long sum_squared(unsigned int n) {
  unsigned long triangle[PASCAL_TRIANGLE_MAX_SIZE][PASCAL_TRIANGLE_MAX_SIZE];
  unsigned long sum = 0;
  generatePascalTriangle(n * 2, triangle);
//  printPascalTriangle(n*2, triangle);
  for (int i = n * 2; i > 0; --i) {
    sum += triangle[i][n - 1];
  }
  return sum;
}

int array_min(int *array, unsigned int n) {
  if (!array) return -1;
  int buffer = array[0];
  for (int i = 1; i < n; ++i) { if (array[i] < buffer) buffer = array[i]; }
  return buffer;
}

int array_max(int *array, unsigned int n) {
  if (!array) return -1;
  int buffer = array[0];
  for (int i = 1; i < n; ++i) { if (array[i] > buffer) buffer = array[i]; }
  return buffer;
}

#define DIVIDERS_NUMBERS_ARR_SIZE 100

unsigned int factorize_count(int n) {
  if (n < 0) n *= -1;
  unsigned int dividersArr[DIVIDERS_NUMBERS_ARR_SIZE], dividersIndex = 0;
  unsigned int primeNumbersIndex = 0;
  for (int i = 1; i <= n; ++i) {
    if (n % i == 0) dividersArr[dividersIndex++] = i;
  }
  for (int i = 0; i < dividersIndex; ++i) {
    unsigned char isPrime = 1;
    for (int j = 2; j < dividersArr[i]; ++j) {
      if (dividersArr[i] % j == 0) {
        isPrime = 0;
        break;
      }
    }
    if (isPrime && dividersArr[i] != 1) primeNumbersIndex++;
  }
  return primeNumbersIndex;
}

void podium(int n, int *arr) {
  int buffer = n;
  while (n % 3 != 0) { n++; }
  n /= 3;
  arr[0] = n;
  arr[1] = n + 1;
  arr[2] = buffer - (2 * n + 1);
}


int main() {
//  int input_array[] = {5, 0, 2, 3, 4, 6, 7, 8,
//                       9, 10, 20, 13, 12, 14, 15,
//                       16, 17, 18, 19, 21, 22, 1};
//  int array_size = 22;
//  printf("%d\n", find_missing_number(input_array, array_size));
  return 0;
}

// conflict line 1
