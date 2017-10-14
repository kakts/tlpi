#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

// bad_exclusive_open_sleep.cを改善し、ファイルopen時に排他処理を行ったバージョン
int main(int argc, char *argv[])
{
  int fd, flags, accessMode;

  fd = open(argv[1], O_RDWR);
  if (fd != -1) {
    // openできた場合
    printf("[PID %ld] File \"%s\" cannot open \n", (long) getpid(), argv[1]);
    close(fd);
  }
  printf("-------%d",  fd);
  // オープン済みのファイルのファイルフラグを参照する
  flags = fcntl(fd, F_GETFL);
  if (flags == -1) {
    // 参照
    errExit("fcntl");
  }

  if (flags & O_SYNC) {
    printf("writes are synchronized\n");
  }

  // ファイルアクセスモードの参照
  // O_RDONLY(0) O_WRONLY(1) O_RDWR(2)を表現するために２ビットが必要
  // 単純なビット演算だけでは正しく参照できない
  // fcntlで得られたflagsをO_ACCMODEでマスクし、その後マクロ定数と比較する
  accessMode = flags & O_ACCMODE;
  if (accessMode == O_WRONLY || accessMode == O_RDWR) {
    printf("file is writable \n");
  }

  exit(EXIT_SUCCESS);
}
