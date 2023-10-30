#include <stdio.h>
#include <math.h>

double roundTo(double n, unsigned char precision) {
  int a = (int)pow(10, precision);
  return round(a * n) / a;
}

double lift_a_car(double leverLen, double humanWeight, double carWeight) {
  return roundTo((humanWeight * leverLen) / (carWeight + humanWeight), 2);
}

double unit_price(double price, unsigned int rollsCount, unsigned int piecesCount) {
  return roundTo(price / (rollsCount * piecesCount) * 100, 2);
}

#define BANKNOTES_TYPES_COUNT 5

int bank_notes(int price) {
  if ((price < 0) || (price % 10)) return -1;
  unsigned int banknotesTypes[BANKNOTES_TYPES_COUNT] = {10, 20, 50, 100, 200};
  unsigned int ans = 0;
  for (int i = BANKNOTES_TYPES_COUNT; i > 0; --i) {
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

void sortArray(int* array, unsigned int n) {
  char isSorted = 1;
  for (int i = 1; i < n; ++i) {
    int buffer;
    if (array[i] < array[i-1]) {
      isSorted = 0;
      buffer = array[i-1];
      array[i-1] = array[i];
      array[i] = buffer;
    }
    if (i == n-1 && !isSorted) {
      i = 1;
      isSorted = 1;
    }
  }
}

int find_missing_number(unsigned int arrSize, int* array) { // blyat, why do not use unsigned int* array
  sortArray(array, arrSize);
  for (int i = 1; i < arrSize; ++i) {
    if (array[i] != array[i-1]+1) return array[i-1]+1;
  }
  return -1;
}

int main() {
  int input_array[] = {3,0,1};
  printf("%d\n", find_missing_number(3,input_array));
  return 0;
}
