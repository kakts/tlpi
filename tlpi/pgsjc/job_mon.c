/**
 * @file job_mon.c
 * 34-5 
 * ジョブコントロール
 * シェルがどのようにパイプラインのコマンド群からジョブ(プロセスグループ）を形成するかの例
 * 
 * 次に示すようにパイプラインにより複数実行可能
 * ./job_mon | ./job_mon | ./job_mon
 */
#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// パイプライン内での位置
static int cmdNum;

// 複数種類のシグナルを処理するハンドラ
static void handler(int sig)
{
    if (getpid() == getpgrp()) {
        // プロセスグループリーダーの場合
        fprintf(stderr, "Terminal FG process group: %ld\n", (long) tcgetpgrp(STDERR_FILENO));
    }
    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n", (long) getpid(), cmdNum, sig, strsignal(sig));

    // SIGTSTPを捕捉しても実行を一時停止しないため、SIGSTOPを送信し直す
    if (sig == SIGTSTP) {
        raise(SIGSTOP);
    }
}


int main(int argc, char *argv[])
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        errExit("sigaction");
    }
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        errExit("sigaction");
    }
    if (sigaction(SIGCONT, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    // stdinが端末なら、パイプラインの先頭プロセスである
    // その場合、見出しを表示しパイプへ書き込む整数を初期化する
    if (isatty(STDIN_FILENO)) {
        fprintf(stderr, "Terminal FG process group: %ld\n", (long) tcgetpgrp(STDIN_FILENO));
        fprintf(stderr, "Command PID PPID PGRP SID\n");
        cmdNum = 0;
    } else {
        // 先頭プロセスではないため、パイプから整数を読み取る
        if (read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0) {
            fatal("read got EOF or error");
        }
    }

    cmdNum++;
    fprintf(stderr, "%4d    %5ld %5ld %5ld %5ld\n", cmdNum, (long) getpid(), (long) getppid(), (long) getpgrp(), (long) getsid(0));

    // 末尾プロセスでない場合、次のプロセスへ整数を書き込む
    if (!isatty(STDOUT_FILENO)) {
        if (write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1) {
            errMsg("write");
        }
    }

    for (;;) {
        pause();
    }
}
