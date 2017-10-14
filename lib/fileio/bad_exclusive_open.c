#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

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

      // 排他的にファイルを生成しない誤った例
      // 2度目のopenを行う前に、他のプロセスが同じファイルを作成した場合
      // つまりプロセスがタイムスライスを使い切り、カーネルスケジューラが他のプロセスを実行した場合に起こり得る
      // またはマルチプロセッサシステムでこのプログラムを同時実行しても起こり得る
      // この場合競合状態となってしまう
      fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
      if (fd == -1) {
        errExit("open");
      }

      printf("[PID %ld] Created file \"%s\" exclusively\n", (long) getpid(), argv[1]);
    }
    exit(EXIT_SUCCESS);
  }

}
