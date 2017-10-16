// コマンドラインパラメータを 1行に1つずつ表示する
#include "../tlpi_hdr.h"

int main(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  exit(EXIT_SUCCESS);
}
