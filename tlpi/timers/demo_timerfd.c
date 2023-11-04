/**
 * @file demo_timerfd.c
 * 23-8
 * timerfd APIの例
 * 
 * 引数にタイマーの初期値とインターバル、プログラムが終了するまでにまつ最大タイムアウト回数(optional)を渡す
 * 
 * タイマーの初期値: 1sec インターバル: 1sec 最大タイムアウト回数: 100回で実行する場合
 *  ./demo_timerfd 1:1 5 
 * 
 */
#include <sys/timerfd.h>
#include <time.h>
#include <stdint.h>
#include "../lib/itimerspec_from_str.h"
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct itimerspec ts;
    struct timespec start, now;
    int maxExp, fd, secs, nanosecs;
    uint64_t numExp, totalExp;
    ssize_t s;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]][max-exp]\n", argv[0]);
    }

    itimerspecFromStr(argv[1], &ts);
    // タイマーの初期値
    maxExp = (argc > 2) ? getInt(argv[2], GN_GT_0, "max-exp") : 1;

    // fd経由のタイマー作成
    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1) {
        errExit("timerfd_create");
    }

    // タイマー開始
    if (timerfd_settime(fd, 0, &ts, NULL) == -1) {
        errExit("timerfd_settime");
    }

    // タイマー開始時刻を取得
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        errExit("clock_gettime");
    }

    for (totalExp = 0; totalExp < maxExp;) {

        /**
         * タイムアウト回数をreadして、タイマー開始からの経過時間、読み取ったタイムアウト回数、合計数を表示
         */

        s = read(fd, &numExp, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            errExit("read");
        }

        totalExp += numExp;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
            errExit("clock_gettime");
        }

        secs = now.tv_sec - start.tv_sec;
        nanosecs = now.tv_nsec - start.tv_nsec;

        // ナノ秒が負の場合、秒を1減らし、ナノ秒を1秒(10^9ナノ秒)増やす
        if (nanosecs < 0) {
            secs--;
            nanosecs += 1000000000;
        }

        printf("%d.%03d: expirations read: %llu; total=%llu\n", secs, (nanosecs + 500000) / 1000000, (unsigned long long)numExp, (unsigned long long)totalExp);
    }
}
