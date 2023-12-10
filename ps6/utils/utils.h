#ifndef ZAP_2023_8748_UTILS_H
#define ZAP_2023_8748_UTILS_H

unsigned char getNumberOfCores();
int milliSleep(long milliseconds);
long getFileSize(FILE* fp);
int max(int a, int b);
int min(int a, int b);
unsigned short normalizeIndex(int i, unsigned short limit);

#endif //ZAP_2023_8748_UTILS_H
