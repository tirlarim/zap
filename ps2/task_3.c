#include <stdio.h>
#include <math.h>

#define BANKNOTES_TYPES_COUNT 5

double roundTo(double n, unsigned char precision) {
  int a = (int)pow(10, precision);
  return round(a*n)/a;
}

double lift_a_car(double leverLen, double humanWeight, double carWeight) {
  return roundTo((humanWeight*leverLen)/(carWeight+humanWeight), 2);
}

double unit_price(double price, unsigned int rollsCount, unsigned int piecesCount) {
  return roundTo(price/(rollsCount*piecesCount)*100, 2);
}

int bank_notes(int price) {
  if ((price < 0) || (price % 10)) return -1;
  unsigned int banknotesTypes[BANKNOTES_TYPES_COUNT] = {10, 20, 50, 100, 200};
  unsigned int ans = 0;
  for (int i = BANKNOTES_TYPES_COUNT; i > 0; --i) {
    while (price >= banknotesTypes[i]) {
      price-=(int)banknotesTypes[i];
      ans+=1;
    }
  }
  return (int)ans;
}

int main() {
//  printf("%d\n", bank_notes(-50));
  return 0;
}
