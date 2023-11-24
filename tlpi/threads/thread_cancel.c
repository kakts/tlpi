/**
 * @file thread_cancel.c
 * 32-1 pthread_cancel()によるキャンセル要求
 */

#include <pthread.h>
#include "tlpi_hdr.h"

static void * threadFunc(void *arg)
{
    int j;

    // システムによってはprintfがキャンセルポイントの可能性あり
    printf("New thread started\n");
    for (j = 1; ; j++) {
        printf("Loop %d\n", j);
        
        // キャンセルポイント
        sleep(1);
    }
    // ここには到達しない
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    int s;
    void *res;

    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }

    // 作成したスレッドをしばらく実行する
    sleep(3);

    s = pthread_cancel(thr);
    if (s != 0) {
        errExitEN(s, "pthread_cancel");
    }

    s = pthread_join(thr, &res);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }

    // pthread_join()によって得られたスレッドの終了ステータスをチェックする
    if (res == PTHREAD_CANCELED) {
        printf("Thread was canceled\n");
    } else {
        printf("Thread was not canceled (should not happen!)\n");
    }

    exit(EXIT_SUCCESS);
}