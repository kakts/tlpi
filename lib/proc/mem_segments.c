// プロセスメモリセグメントに置かれる変数
#include <stdio.h>
#include <stdlib.h>


// 未初期化データセグメント
char globBuf[65536];

// 初期化済みデータセグメント
int primes[] = {2, 3, 5, 7};

// square()のフレーム上に割り当てられる
static int square(int x)
{
  int result;

  result = x * x;

  // レジスタ経由で戻り値を返す
  return result;
}

// doCalc()のフレーム上に割り当てられる
static void doCalc(int val)
{
  printf("The square of %d is %d\n", val, square(val));

  if (val < 1000) {
    // doCalc()のフレーム上に割り当てられる
    int t;

    t = val * val * val;
    printf("The cube of %d is %d\n", val, t);
  }
}

// main()のフレーム上に割り当てられる
int main(int argc, char *argv[])
{
  // 初期化済みデータセグメント
  static int key = 9973;

  // 未初期化データセグメント
  static char mbuf[10240000];

  // main()のフレーム上に割り当てられる
  char *p;

  // ヒープセグメント上の領域を指す
  p = malloc(1024);

  doCalc(key);
  exit(EXIT_SUCCESS);
}
