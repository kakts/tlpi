/**
 * @file thread_cleanup.c
 * 32-2 クリーンナップハンドラによる、スレッドキャンセル前の処理
 * 
 * コマンド引数に何も与えない場合: main()がスレッドをキャンセルする
 * pthread_cond_wait()がキャンセルポイントとなる
 * 
 * コマンド引数を与えた場合: globに以外を代入後、条件変数による通知が行われる
 */
#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
// 状態を表す変数
static int glob = 0;

/**
 * argが刺すメモリを解放し mutexをアンロックするクリーンナップハンドラ
 */
static void cleanupHandler(void *arg)
{
    int s;

    printf("cleanup: freeing block at %p\n", arg);
    free(arg);
    printf("cleanup: unlocking mutex\n");
    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_unlock");
    }
}

static void * threadFunc(void *arg)
{
    int s;

    // スレッド割り当てるバッファ
    void *buf = NULL;

    // キャンセルポイントではない
    buf = malloc(0x10000);
    printf("thread: allocated memory at %p\n", buf);

    // キャンセルポイントではない
    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_lock");
    }

    // クリーンナップハンドラを登録する
    pthread_cleanup_push(cleanupHandler, buf);
    while (glob == 0) {
        // キャンセルポイント
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0) {
            errExitEN(s, "pthread_cond_wait");
        }
    }

    printf("thread: condition wait loop completed\n");
    // クリーンナップハンドラをポップし、実行する
    pthread_cleanup_pop(1);
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t thr;
    void *res;
    int s;

    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }

    sleep(2);

    // スレッドのキャンセル
    if (argc == 1) {
        printf("main: about to cancel thread\n");
        s = pthread_cancel(thr);
        if (s != 0) {
            errExitEN(s, "pthread_cancel");
        }
    } else {
        // 条件変数による通知
        printf("main: about to signal condition variable\n");
        glob = 1;
        s = pthread_cond_signal(&cond);
        if (s != 0) {
            errExitEN(s, "pthread_cond_signal");
        }
    }
    
    s = pthread_join(thr, &res);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }
    if (res == PTHREAD_CANCELED) {
        printf("main: thread was canceled\n");
    } else {
        printf("main: thread terminated normally\n");
    }
    exit(EXIT_SUCCESS);
}
