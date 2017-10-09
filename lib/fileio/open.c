#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
  int fd;

  // 読み書き両用に既存もしくは新規ファイルをオープンし、既存の場合はファイルサイズを0(O_TRUNC)にする
  // パーミッションはオーナーのみが読み書き可能で、それ以外は許容しない
  fd = open("myfile", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    errExit("open");
  }
  printf("open success.\n");
  if (close(fd) == -1) {
    errExit("close");
  }
  printf("close success.\n");
}
