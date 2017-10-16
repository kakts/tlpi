// cのグローバル変数 char **environを用い、全ての環境変数へアクセスするコード
// 出力結果はprintenvとおなじになる

#include "../tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[])
{
  char **ep;

  for (ep = environ; *ep != NULL; ep++) {
    puts(*ep);
  }

  exit(EXIT_SUCCESS);
}
