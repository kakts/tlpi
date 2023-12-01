/**
 * @file catch_SIGHUP.c
 * 34-3 シェルがSIGHUPを受信し、ジョブへSIGHUPを伝播する処理の例
 * 
 * コマンドライン引数を与えると、子プロセスが自身のプロセスグループを変更する
 * これにより、シェルと同一セッション内に存在していても、シェルは自身が作成したプロセスグループ
 * 以外へはSIGHUPを伝播しないことを確認できる
 */
#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    pid_t childPid;
    struct sigaction sa;

    // stdoutのバッファリング停止
    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    // SIGHUPのシグナルハンドラを設定する
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    childPid = fork();
    if (childPid == -1) {
        errExit("fork");
    }

    // 子プロセスで引数が与えられた場合
    if (childPid == 0 && argc > 1) {
        // 新規プロセスグループへ移動する
        if (setpgid(0, 0) == -1) {
            errExit("setpgid");
        }
    }

    printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n", (long) getpid(), (long) getppid(), (long) getpgrp(), (long) getsid(0));

    // まだ動作していればSIGALRMがプロセス終了を保証する
    alarm(60);

    // シグナルを待つ
    for (;;) {
        pause();
        printf("%ld: caught SIGHUP\n", (long) getpid());
    }
}
