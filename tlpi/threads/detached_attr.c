/**
 * detached_attr.c
 * 
 * 29-2 デタッチ属性を設定したスレッドの作成
 * デタッチ属性を設定すると、スレッド作成時点でデタッチ設定が付与される
 * 
 * pthread_joinによってスレッドの終了を待つことはできない
 */

#include <pthread.h>
#include "tlpi_hdr.h"

static void * threadFunc(void *x)
{
    return x;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    // スレッド属性を設定するための変数
    pthread_attr_t attr;
    int s;

    // デフォルト値を使用してスレッド属性を初期化する
    s = pthread_attr_init(&attr);
    if (s != 0) {
        errExitEN(s, "pthread_attr_init");
    }

    // デタッチ属性を設定する
    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (s != 0) {
        errExitEN(s, "pthread_attr_setdetachstate");
    }

    // スレッドの作成
    s = pthread_create(&thr, &attr, threadFunc, (void *) 1);
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }

    // スレッド属性を破棄する
    s = pthread_attr_destroy(&attr);
    if (s != 0) {
        errExitEN(s, "pthread_attr_destroy");
    }

    // スレッドの終了を待つ
    s = pthread_join(thr, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }
    
    exit(EXIT_SUCCESS);
}
