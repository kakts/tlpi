// setjmp()と longjmp()の使用例

// setjmp()は、第1引数へ代入する情報
// プログラムカウンタレジスタ: 現在実行中の機械語命令
// スタックポインタレジスタ: スタックトップ

// 上記のsetjmp()によって設定された情報をもとに、longjmp()は次の処理を実行する
// ・スタック上に置かれた、現在実行中の longjmp() から先に実行された setjmp() までの、 全経由関数のスタックフレームを破棄する。
//   実際には、スタックポインタに env が持つ値を 再代入する。この処理をスタック巻き戻し（unwind the stack）とも言う。
// ・先の setjmp() から実行を再開できるように、プログラムカウントを変更する。ここでも env が持つ値が用いられる。

#include <setjmp.h>
#include "../tlpi_hdr.h"

static jmp_buf env;

static void f2(void)
{
  longjmp(env, 2);
}

static void f1(int argc)
{
  if (argc == 1) {
    longjmp(env, 1);
  }
  f2();
}

int main(int argc, char *argv[])
{
  // setjmp()は、設定したときには0が返る。 longjmp()からのリターン時には0以外を返す
  switch(setjmp(env)) {
  case 0:
    printf("Calling f1() after initial setjmp()\n");
    f1(argc);
    break;

  case 1:
    printf("We jumped back from f1()\n");
    break;

  case 2:
    printf("We jumped back from f2()\n");
    break;
  }

  exit(EXIT_SUCCESS);
}
