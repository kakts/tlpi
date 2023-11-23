/**
 * @file prod_condvar.c
 * PThreadsによる、条件変数を用いた producer-consumerモデル。
 */
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

// mutexの初期化
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
// 条件変数の初期化
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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

        // consumerに他のユニットが利用可能であることを知らせる
        avail++;

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_unlock");
        }

        // 停止しているconsumerを起こす
        s = pthread_cond_signal(&cond);
        if (s != 0) {
            errExitEN(s, "pthread_cond_signal");
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
        int s = pthread_create(&tid, NULL, producer, argv[j]);
        if (s != 0) {
            errExitEN(s, "pthread_create");
        }

    }

    // 全ての利用可能なユニットを消費するためのループ
    
    // いままでに消費されたユニット数
    int nuMConsumed = 0;
    bool done = false;

    for (;;) {
        int s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_lock");
        }

        // 利用可能なユニットがない場合は、条件変数を待つ
        while (avail == 0) {
            // condという条件変数を使ってスレッドを待機させる
            s = pthread_cond_wait(&cond, &mtx);
            if (s != 0) {
                errExitEN(s, "pthread_cond_wait");
            }
        }

        // この段階でmtxはロックされている

        // すべてのユニットを消費する
        while (avail > 0) {
            // ユニットを消費する
            nuMConsumed++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t), nuMConsumed);

            done = nuMConsumed >= totRequired;
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
