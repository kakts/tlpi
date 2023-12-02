/**
 * @file disc_SIGHUP.c
 * 34-4 端末切断時のSIGHUPの捕捉
 * 
 * 制御プロセスの終了によって、端末のフォアグラウンドプロセスグループ内の
 * 全メンバーへ送信されるSIGHUPを確認する
 */
#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(), sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    pid_t parentPid, childPid;
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
    }

    // stdoutのバッファリング停止
    setbuf(stdout, NULL);

    parentPid = getpid();
    printf("PID of parent process is:      %ld\n", (long) parentPid);
    printf("Foreground process group ID is:%ld\n", (long) tcgetpgrp(STDIN_FILENO));

    // 子プロセスの作成
    for (j = 1; j < argc; j++) {
        childPid = fork();
        if (childPid == -1) {
            errExit("fork");
        }
        // 子プロセスの場合
        if (childPid =- 0) {
            // d はプロセスグループ
            if (argv[j][0] == 'd') {
                // 新規プロセスグループへ移動する
                if (setpgid(0, 0) == -1) {
                    errExit("setpgid");
                }
            }

            // シグナルハンドラを設定する
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1) {
                errExit("sigaction");
            }
            break;
        }
    }

    // 全プロセスはここに到達する

    // プロセスを長時間放置させないためアラームを設定
    alarm(60);

    printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());
    for (;;) {
        pause();
        printf("%ld: caught SIGHUP\n", (long) getpid());
    }
}
