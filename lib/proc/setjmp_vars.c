// setjmp()とlongjmp()の使用例2
// コンパイル時に最適化が施された場合 setjmp() longjmp()を使ったコードは
// プログラム内で命令の順序が前後したり、変数がメモリ上に置かれるため意図しない動作となる場合がある

// このプログラムは最適化をしない場合とした場合で結果が変わる
// コンパイラが異なれば、最適化後の結果も変わる

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void doJump(int nvar, int rvar, int vvar)
{
  printf("Inside doJump(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
  longjmp(env, 1);
}

int main(int argc, char *argv[])
{
  int nvar;

  // 可能ならばレジスタに置く変数
  register int rvar;

  // TLPI Chapter6.8を参照
  // volatileは 最適化を抑制するので、最適化による意図しない動作を防げる
  volatile int vvar;

  nvar = 111;
  rvar = 222;
  vvar = 333;


  if (setjmp(env) == 0) {
    // setjmp()後
    nvar = 777;
    rvar = 888;
    vvar = 999;
    doJump(nvar, rvar, vvar);
  } else {
    printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
  }

  exit(EXIT_SUCCESS);
}
