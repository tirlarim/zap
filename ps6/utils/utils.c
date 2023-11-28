#include <stdio.h>
#include <time.h>
#include <errno.h>


int milliSleep(long milliseconds) {
  struct timespec ts;
  int res;
  if (milliseconds < 0) {
    errno = EINVAL;
    return -1;
  }
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
  return res;
}

long getFileSize(FILE* fp) {
  long size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}
