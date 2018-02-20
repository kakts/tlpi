#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
  int fd, j, numBlocks;
  char shellCmd[CMD_SIZE]; // system()へ渡すコマンド文字列

  char buf[BUF_SIZE]; // ファイルへ書き込むランダムバイト列

  if (argc < 2 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s temp-file [num-1kB-blocks] \n", argv[0]);
  }

  numBlocks = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-1kB-blocks") : 100000;

  fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    errExit("open");
  }

  // ファイル名を削除
  if (unlink(argv[1]) == -1) {
    errExit("unlink");
  }

  for (j = 0; j < numBlocks; j++) {
    // 大量のゴミデータを書き込む
    if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
      fatal("partial/failed write");
    }
  }

  snprintf(shellCmd, CMD_SIZE, "df -k %s", argv[1]);
  system(shellCmd); // ファイルシステム内での消費量を表示

  if (close(fd) == -1) {
    errExit("close");
  }

  printf("**********Closed file description\n");

  system(shellCmd);

  exit(EXIT_SUCCESS);
}
