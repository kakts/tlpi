/**
 * @file thread_incr.c
 * 30-1 同期を用いず、同じグローバル変数をインクリメントする2つのスレッドの例
 */
#include <pthread.h>
#include "tlpi_hdr.h"

// volatileにより `glob` の算術演算に対する最適化を抑制する
static volatile int glob = 0;

// globを argに指定された回数インクリメントする
static void * threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    // グローバル変数の値をローカルのメモリに保持し、それをインクリメントする
    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    // コマンドライン引数の取得
    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    // スレッドの作成
    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }

    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }

    // スレッド1の終了を待つ
    s = pthread_join(t1, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }
    // スレッド2の終了を待つ
    s = pthread_join(t2, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
