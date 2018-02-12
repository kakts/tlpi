#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
  printf("Create directory.\n");
  mkdir("test_01", 0777);
  mkdir("test_02", 0777);
  printf("Finished\n");

  printf("Delete test_02 directory\n");
  rmdir("test_02");
  printf("Finished\n");

  return 0;
}
