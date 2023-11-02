/**
 * @file t_nanosleep.c
 * 23-3 nanosleep()の使用例
 * 
 * 引数に指定した時間だけプロセスを停止させる。
 * 停止中、SIGINTを実行すると、シグナルハンドラが呼ばれ、nanosleep()が停止される。
 * nanosleep()が停止された場合、戻り値のremainに残り時間が格納され、その情報をもとに再度nanosleep()を実行させ
 * 指定した時間が完了するまで繰り返す.
 * 
 * コマンド実行例 10秒間sleepさせる場合
 *  ./t_nanosleep 10 0
 * 
 * この間に　Crtl-C を実行すると、シグナルハンドラが呼ばれ、nanosleep()が停止される。その時点でのsleepのステータスが表示される
 */

#define _POSIX_C_SOURCE 199309
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "../lib/tlpi_hdr.h"

/**
 * nanosleep() は、指定された時間だけプロセスを停止する
 */
static void sigintHandler(int sig)
{
    return;
}

int main(int argc, char *argv[])
{
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s secs nanosecs\n", argv[0]);
    }

    request.tv_sec = getLong(argv[1], 0, "secs");
    request.tv_nsec = getLong(argv[2], 0, "nanosecs");

    /**
     * SIGINT ハンドラはnanosleep()に割り込んで良い
     */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    // nanosleep() を呼び出す前に開始時刻を取得する
    if (gettimeofday(&start, NULL) == -1) {
        errExit("gettimeofday");
    }

    for (;;) {
        s = nanosleep(&request, &remain);

        // nanosleep() がエラーになった場合
        if (s == -1 && errno != EINTR) {
            errExit("nanosleep");
        }

        // nanosleep() が正常に終了した場合の時刻を取得
        if (gettimeofday(&finish, NULL) == -1) {
            errExit("gettimeofday");
        }

        // 開始時間と終了時間の差分 つまりsleepしていたおおよその時間を表示
        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec) / 1000000.0);

        // nanosleep()が正常に終了した場合、ループ終了
        if (s == 0) {
            break;
        }

        // nanosleep実行によって残りの時間が変更された場合、残りの時間を表示
        printf("Remaining: %2ld.%09ld\n", (long) remain.tv_sec, remain.tv_nsec);

        // 次のイテレーションで、残り時間を再度nanoleep()に渡す
        request = remain;
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);
}