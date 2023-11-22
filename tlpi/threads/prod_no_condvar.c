/**
 * @file prod_no_condvar.c
 * PThreadsによる、条件変数を用いないシンプルな producer-consumerモデル。
 */

#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

// mutexの初期化
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static int avail = 0;

static void *producer(void *arg)
{
    int cnt = atoi((char *) arg);

    for (int j = 0; j < cnt; j++) {
        sleep(1);

        int s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_lock");
        }

        avail++;

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_unlock");
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    time_t t = time(NULL);

    // 全てのスレッドが生成するユニット数
    int totRequired = 0;

    // 全てのスレッドを作成する
    for (int j = 1; j < argc; j++) {
        totRequired += atoi(argv[j]);

        pthread_t tid;
        // スレッド生成
        int s = pthread_create(&tid, NULL, producer, argv[j]);
        if (s != 0) {
            errExitEN(s, "pthread_create");
        }

    }

    // 利用可能なユニットをチェックするためにポーリングループを実行する
    
    // いままでに消費されたユニット数
    int numConsumed = 0;
    bool done = false;

    for (;;) {
        int s = pthread_mutex_lock(&mtx);

        if (s != 0) {
            errExitEN(s, "pthread_mutex_lock");
        }

        while (avail > 0) {
            // ユニットを消費する
            numConsumed++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t), numConsumed);
            done = numConsumed >= totRequired;
        }

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_unlock");
        }

        if (done) {
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
