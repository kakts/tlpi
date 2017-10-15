#include <unistd.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

// truncateのサンプルコード
int main(int argc, char *argv[])
{
  int result_truncate, size_length;

  // 変更後のファイルサイズを指定する
  size_length = atoi(argv[2]);

  printf("size_length: %d\n", size_length);

  result_truncate = truncate("test.txt", size_length);
  if (result_truncate == -1) {
    printf("truncate file failed\n");
    errExit("truncate");
  }

  printf("success\n");
  exit(EXIT_SUCCESS);
}
