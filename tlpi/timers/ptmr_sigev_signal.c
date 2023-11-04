/**
 * @file ptmr_sigev_signal.c
 * 23-5 シグナルによるPOSIXタイマ通知
 */
#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "curr_time.h"
#include "itimerspec_from_str.h"
#include "../lib/tlpi_hdr.h"

// タイマー通知シグナル
#define TIMER_SIG SIGRTMAX

static void handler(int sig, siginfo_t *si, void *uc)
{
    timer_t *tidptr;

    tidptr = si->si_value.sival_ptr;

    printf("[%s] Got signal %d\n", currTime("%T"), sig);
    printf("    *sival_ptr         = %ld\n", (long)*tidptr);
    printf("    timer_getoverrun() = %d\n", timer_getoverrun(*tidptr));
}

int main(int argc, char *argv[])
{
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent sev;
    timer_t *tidlist;

    int j;

    if (argc < 2) {
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);
    }

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(TIMER_SIG, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    /**
     * コマンドライン引数1つずつにタイマーを作成し開始する
     */

    // シグナルによる通知
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMER_SIG;

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        sev.sigev_value.sival_ptr = &tidlist[j];

        // ハンドラからタイマーIDを参照可能にする
        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1) {
            errExit("timer_create");
        }
        printf("Timer ID: %ld (%s)\n", (long)tidlist[j], argv[j + 1]);

        // タイマーを開始する
        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1) {
            errExit("timer_settime");
        }
    }

    // タイマーからのシグナルを待つ
    for (;;) {
        pause();
    }

}