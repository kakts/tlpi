/**
 * @file intquit.c
 * 20-2 2種類のシグナルを処理するシグナルハンドラ
 */

#include <signal.h>
#include "../lib/tlpi_hdr.h"

static void sigHandler(int sig)
{
    static int count = 0;

    // 注意このハンドラは非同期シグナルセーフではない printf() exit()を使用している

    if (sig == SIGINT) {
        count++;
        printf("Caught SIGINT(%d)\n", count);
        return;
    }

    // SIGQUITの場合 メッセージ表示し、プロセス終了
    printf("Caught SIGQUIT - that's all folks!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // SIGINT SIGQUITにハンドラを設定
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        errExit("signal");
    }
    if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
        errExit("signal");
    }

    for (;;) {
        pause(); // シグナルを受け取るまで待機
    }
}