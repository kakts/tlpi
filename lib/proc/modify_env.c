// 環境変数を指定した値に変更する


#define _GNU_SOURCE  // <stdlib.h>内の宣言を有効にする MacOSでは動かない
#include <stdlib.h>
#include "../tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[])
{
  int j;
  char **ep;

  // 環境の全削除
  clearenv();

  // putenv() 実行プロセスの環境へ新たな環境変数を追加
  for (j = 1; j < argc; j++) {
    if (putenv(argv[j]) != 0) {
      errExit("putenv: %s", argv[j]);
    }
  }

  // setemv() 新たにメモリを割り与え、 name=value形式の文字列をコピーし環境変数を追加
  // setenv()は、nameが既に存在し、第3引数が0に設定されていた場合、環境を変更しない
  if (setenv("GREET", "Hello world", 0) == -1) {
    errExit("setenv");
  }

  unsetenv("BYE");

  // environにNULLを代入したままの場合 *environが向こうになるためループは期待通りに動作しない
  // setenv() と putenv() が environ が NULL であることを検知し、新たな環境を作成し environ へ代入し直すとすれば、ループは意図した通りに動作します。
  for (ep = environ; *ep != NULL; ep++) {
    puts(*ep);
  }

  exit(EXIT_SUCCESS);
}
