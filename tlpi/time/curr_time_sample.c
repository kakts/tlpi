#include "../lib/tlpi_hdr.h"
#include "curr_time.h"

int main(int argc, char *argv[])
{
  char *test1, *test2;

  test1 = currTime(NULL);
  printf("test1: %s\n", test1);
  // 曜日を表示
  test2 = currTime("%a: %A");


  printf("test2: %s\n", test2);
}
