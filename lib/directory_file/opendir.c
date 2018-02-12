#include <dirent.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

int main(int argc, char *argv[])
{
  struct dirent *dirst;

  printf("Create directory.\n");
  mkdir("test_01", 0777);
  mkdir("test_01/inner", 0777);
  printf("Finished\n");

  printf("Open a directory.\n");
  // Open a directory.
  DIR *dp = opendir("test_01");
  while((dirst = readdir(dp)) != NULL)
  {
    // print the name of directory and name length
    printf("%s\n", dirst->d_name);
    printf("%d\n", dirst->d_namlen);
  }

  return 0;
}
