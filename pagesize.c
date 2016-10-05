#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  printf("%d\n", getpagesize());
  return 0;
}
