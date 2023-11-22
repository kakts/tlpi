/**
 * @file pthread_incr_mutex.c
 * 30-2 mutexによるグローバル変数の保護
 */

#include <pthread.h>
#include "tlpi_hdr.h"

static int glob = 0;
// mutexの初期化
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


static void * threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j, s;

    for (j = 0; j < loops; j++) {
        // mutexのロック
        s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_lock");
        }

        // globの値をローカル変数にコピーし、それをインクリメントする
        loc = glob;
        loc++;
        glob = loc;

        // mutexのアンロック
        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_unlock");
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

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
