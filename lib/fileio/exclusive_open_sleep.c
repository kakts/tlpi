#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

// bad_exclusive_open_sleep.cを改善し、ファイルopen時に排他処理を行ったバージョン
int main(int argc, char *argv[])
{
  int fd;

  fd = open(argv[1], O_WRONLY);
  if (fd != -1) {
    // openできた場合
    printf("[PID %ld] File \"%s\" already exists \n", (long) getpid(), argv[1]);
    close(fd);
  } else {
    if (errno != ENOENT) {
      // 予期せぬエラーでopenできなかった
      errExit("open");
    } else {

      printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long) getpid(), argv[1]);
      if (argc > 2) {
        sleep(5);
        printf("[PID %ld] Done sleeping\n", (long) getpid());
      }

      // O_EXCL は、O_CREATフラグとともに用い、既にファイルが存在する場合は新規作成・openせず、
      // エラーとする (errnoにはEEXISTが代入される)
      // すなわち、open()したプロセスこそがこのファイルを作成したプロセスであることを保証でき、
      // ファイルが存在するか田舎の確認はアトミックに行われる。
      fd = open(argv[1], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
      if (fd == -1) {
        errExit("open");
      }

      printf("[PID %ld] Created file \"%s\" exclusively\n", (long) getpid(), argv[1]);
    }
    exit(EXIT_SUCCESS);
  }

}
