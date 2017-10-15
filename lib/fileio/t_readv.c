#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

// readvのサンプルコード
int main(int argc, char *argv[])
{
  int fd;

  // バッファの配列
  struct iovec iov[3];

  // 先頭バッファ
  struct stat myStruct;

  // 2番目のバッファ
  int x;
#define STR_SIZE 100

  // 末尾のバッファ
  char str[STR_SIZE];

  ssize_t numRead, totRequired;

  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s file\n", argv[0]);
  }

  // 読み取り専用で開く
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    errExit("open");
  }

  totRequired = 0;

  iov[0].iov_base = &myStruct;
  iov[0].iov_len = sizeof(struct stat);
  totRequired += iov[0].iov_len;

  iov[1].iov_base = &x;
  iov[1].iov_len = sizeof(x);
  totRequired += iov[1].iov_len;

  iov[2].iov_base = str;
  iov[2].iov_len = STR_SIZE;
  totRequired += iov[2].iov_len;


  numRead = readv(fd, iov, 3);
  if (numRead == -1) {
    errExit("readyv");
  }

  // 要求した総バイト数よりも読み込みバイト数が少ない場合
  if (numRead < totRequired) {
    printf("Read fewer bytes than requested \n");
  }

  printf("total bytes requested %ld; bytes read: %ld \n", (long) totRequired, (long) numRead);
  exit(EXIT_SUCCESS);
}
