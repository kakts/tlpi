/**
 * @file real_timer.c
 * 23-1 実時間タイマーの使用例
 * 
 *  ./real_timer 1 800000 1 0 初期値1.8sec、インターバル1sec
 */

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "../lib/tlpi_hdr.h"

// SIGALRM受信時に0以外の値をセットする
static volatile sig_atomic_t gotAlarm = 0;

/**
 * 実時間を参照・表示する
 * includeTimerがtrueの時、ITIMER_REALタイマーの現在値とインターバルも表示
 */
static void displayTimes(const char *msg, Boolean includeTimer)
{
    struct itimerval itv;
    static struct timeval start;
    struct timeval curr;
    static int callNum = 0;

    if (callNum == 0) {
        if (gettimeofday(&start, NULL) == -1) {
            errExit("gettimeofday");
        }
    }

    // 20行ごとにヘッダを表示
    if (callNum % 20 == 0) {
        printf("       Elapsed   Value Interval\n");
    }

    if (gettimeofday(&curr, NULL) == -1) {
        errExit("gettimeofday");
    }

    printf("%-7s %6.2f", msg, curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

    if (includeTimer) {
        if (getitimer(ITIMER_REAL, &itv) == -1) {
            errExit("getitimer");
        }
        printf(" %6.2f %6.2f", itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0, itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
    }

    printf("\n");
    callNum++;
}

static void signalrmHandler(int sig)
{
    gotAlarm = 1;
}

int main(int argc, char *argv[])
{
    struct itimerval itv;
    clock_t prevClock;

    // シグナル補足回数上限
    int maxSigs;
    // シグナル補足回数
    int sigCnt;

    struct sigaction sa;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [secs [usecs [int-secs [int-usecs]]]]\n", argv[0]);
    }

    sigCnt = 0;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = signalrmHandler;

    // コマンドライン引数をもとにタイマーを設定
    itv.it_value.tv_sec = (argc > 1) ? getLong(argv[1], 0, "secs") : 2;
    itv.it_value.tv_usec = (argc > 2) ? getLong(argv[2], 0, "usecs") : 0;

    itv.it_interval.tv_sec = (argc > 3) ? getLong(argv[3], 0, "int-secs") : 0;
    itv.it_interval.tv_usec = (argc > 4) ? getLong(argv[4], 0, "int-usecs") : 0;

    // シグナルを3回受信すると終了、インターバルなしの場合は一度で終了
    maxSigs = (itv.it_interval.tv_sec == 0 && itv.it_interval.tv_usec == 0) ? 1 : 3;

    displayTimes("START:", FALSE);
    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
        errExit("setitimer");
    }

    prevClock = clock();
    sigCnt = 0;

    for (;;) {
        // 内側のループは少なくとも0.5secはCPU時間を消費する
        while (((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5) {
            if (gotAlarm) {
                gotAlarm = 0;
                displayTimes("ALARM:", TRUE);

                sigCnt++;
                if (sigCnt >= maxSigs) {
                    printf("That's all folks\n");
                    exit(EXIT_SUCCESS);
                }
            }
        }

        prevClock = clock();
        displayTimes("Main: ", TRUE);
    }

}