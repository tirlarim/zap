#include <stdio.h>
#include <math.h>

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

int main() {
//  printf("%.4f\n", unit_price(5.63, 20, 200));
  return 0;
}
