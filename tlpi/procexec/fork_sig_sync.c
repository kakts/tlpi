/**
 * @file fork_sig_sync.c
 * 
 * 24-6 シグナルによる同期を用いた競合状態防止
 * 親プロセスが子プロセスの処理完了を待つ
 */
#include <signal.h>
#include "curr_time.h"
#include "tlpi_hdr.h"

// 同期に使用するシグナル　
#define SYNC_SIG SIGUSR1

static void handler(int sig)
{
    // 何もしない
}

int main(int argc, char const *argv[])
{
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    // stdoutのバッファリング停止
    setbuf(stdout, NULL);

    // SYNC_SIGシグナルをブロックするための設定を行い、その設定が成功したかを確認する
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG); // 同期用シグナルをブロックするマスクを作成
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1) {
        errExit("sigprocmask");
    }

    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0: // child
        printf("[%s %ld] Child started - doing some work\n", currTime("%T"), (long)getpid());
        sleep(2); // 子プロセスが親プロセスより先に終了することを確認するためにsleep()を挟む
        printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long)getpid());
        // 親プロセスに対してシグナルを送信
        if (kill(getppid(), SYNC_SIG) == -1) {
            errExit("kill");
        }

        // 子プロセスの終了
        _exit(EXIT_SUCCESS);

    default:
        printf("[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long)getpid());

        sigemptyset(&emptyMask);
        // SYNC_SIGシグナルを受信するまで待機
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
            errExit("sigsuspend");
        }
        printf("[%s %ld] Parent got signal\n", currTime("%T"), (long)getpid());

        // 必要であれば、変更前のシグナルマスクもえる
        if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1) {
            errExit("sigprocmask");
        }

        exit(EXIT_SUCCESS);
    }
}
