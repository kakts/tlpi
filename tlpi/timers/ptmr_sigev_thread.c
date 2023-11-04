/**
 * @file ptmr_sigev_thread.c
 * 23-7 スレッド起動によるPOSIXタイマー通知
 * 
 * タイマーのタイムアウト時に、指定した関数を別スレッドとして起動させる
 */

#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "curr_time.h"
#include "itimerspec_from_str.h"
#include "../lib/tlpi_hdr.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


// 全タイマーのタイムアウト回数
static int expireCnt = 0;

// スレッド通知用の関数
static void threadFunc(union sigval sv)
{
    timer_t *tidptr;
    int s;

    tidptr = sv.sival_ptr;

    printf("[%s] Thread notify\n", currTime("%T"));
    printf("    timer ID=%ld\n", (long) *tidptr);
    printf("    timer_getoverrun()=%d\n", timer_getoverrun(*tidptr));

    /**
     * メインスレッドと共有するカウンタをインクリメントし、条件変数によりメインスレッドへ通知
     */

    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_lock");
    }
    expireCnt += 1 + timer_getoverrun(*tidptr);

    // expireCntの操作が終わったのでunlockする
    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_unlock");
    }

    // 条件変数によりメインスレッドへ通知
    s = pthread_cond_signal(&cond);
    if (s != 0) {
        errExitEN(s, "pthread_cond_signal");
    }
}

int main(int argc, char *argv[])
{
    struct sigevent sev;
    struct itimerspec ts;
    timer_t *tidlist;
    int s, j;

    if (argc < 2) {
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);
    }

    // タイマー通知をスレッドにより行うように設定
    // timer_t型の要素を格納するためのメモリ領域を動的に確保
    // 引数で指定された要素分のメモリを確保する
    // callocにより、確保されたメモリ領域は0初期化される
    tidlist = calloc(argc - 1, sizeof(timer_t));
    if (tidlist == NULL) {
        errExit("calloc");
    }

    // スレッド起動による通知
    sev.sigev_notify = SIGEV_THREAD;
    // スレッド関数
    sev.sigev_notify_function = threadFunc;
    // pthread_attr_t でスレッド属性を指定する場合は、スレッド関数の引数を指定する
    sev.sigev_notify_attributes = NULL;

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        // threadFunc()への引数
        sev.sigev_value.sival_ptr = &tidlist[j];

        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1) {
            errExit("timer_create");
        }

        printf("Timer ID: %ld (%s)\n", (long)tidlist[j], argv[j + 1]);

        // タイマーのセット
        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1) {
            errExit("timer_settime");
        }
    }

    /**
     * メインスレッドは条件変数により、起動されたスレッド関数からの通知を待つ
     * ユーザにタイムアウトがわかるようにメッセージ表示
     */
    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_lock");
    }

    for (;;) {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0) {
            errExitEN(s, "pthread_cond_wait");
        }
        printf("main(): expireCnt = %d\n", expireCnt);
    }


}