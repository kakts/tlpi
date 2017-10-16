#include <stdlib.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"


// mkstempのサンプル mkstemp()に渡す文字列はxxxxxxを含まなければならない
// この6文字が一意な文字列に置き換わる
// 文字が置き換えられるため、templateは文字列定数でなく文字配列で無くてはならない
int main(int argc, char *argv[])
{
  int fd;

  char template[] = "/tmp/somestringxxxxxx";

  fd = mkstemp(template);
  if (fd == -1) {
    errExit("mkstemp");
  }

  printf("Generated filename was: %s\n", template);
  sleep(5);
  // unlinkによりファイル名は見えなくなるが、ファイルが実際に削除されるのは close()後
  printf("Unlinked filename was: %s\n", template);
  unlink(template);

  // read() write()などI/O実行


  if (close(fd) == -1) {
    errExit("close");
  }

  printf("Done");
  exit(EXIT_SUCCESS);
}
