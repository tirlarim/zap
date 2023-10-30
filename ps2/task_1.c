#include <stdio.h>
#include <math.h>

double roundTo(double n, unsigned char precision) {
  int a = (int)pow(10, precision);
  return round(a*n)/a;
}

double lift_a_car(double leverLen, double humanWeight, double carWeight) {
  return roundTo((humanWeight*leverLen)/(carWeight+humanWeight), 2);
}

int main() {
//  printf("%.4f\n", lift_a_car(2, 80, 1400));
  return 0;
}
