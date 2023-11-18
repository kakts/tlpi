/**
 * @file t_clone.c
 * 28-3 clone()を使って子プロセスを作成する
 */
#define _GNU_SOURCE

#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include "tlpi_hdr.h"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

/**
 * clone()による子プロセスの実行開始点
 */
static int childFunc(void *arg)
{
    if (close(*((int *) arg)) == -1) {
        errExit("close");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // 子プロセス用のスタックサイズ
    const int STACK_SIZE = 65536;
    // 先頭アドレス
    char *stack;
    // 末尾アドレス
    char *stackTop;

    int s, fd, flags;

    // 子プロセスがこのファイルディスクリプタをクローズする
    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        errExit("open");
    }

    // コマンドライン引数で指定されていれば、親子プロセスでファイルディスクリプタテーブルを共有する
    flags = (argc > 1) ? CLONE_FILES : 0;

    // 子プロセス用のスタックを割り当てる
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        errExit("malloc");
    }
    // スタックは下位方向へ成長すると仮定
    stackTop = stack + STACK_SIZE;

    /**
     * CHILD_SIGがデフォルトでプロセスを終了させるシグナルの場合は無視する
     * しかし終了ステータスは必要なためSIGCHLDは無視しない
     */
    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD) {
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR) {
            errExit("signal");
        }
    }

    // 子プロセスの作成 childFunc()から実行開始
    if (clone(childFunc, stackTop, flags | CHILD_SIG, (void *) &fd) == -1) {
        errExit("clone");
    }

    // 親プロセスはここに到達する
    // 子プロセスの終了を待つ
    // SIGCHLD以外のシグナルにも対応するため、＿WCLONEを指定
    if (waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1) {
        errExit("waitpid");
    }
    printf("child has terminated\n");

    // 子プロセスのクローズが親プロセスにも反映されるかどうか
    s = write(fd, "x", 1);
    if (s == -1 && errno == EBADF) {
        printf("file descriptor %d has been closed\n", fd);
    } else if (s == -1) {
        printf("write() on file descriptor %d failed unexpectedly (%s)\n", fd, strerror(errno));
    } else {
        printf("write() on file descriptor %d succeeded\n", fd);
    }
    exit(EXIT_SUCCESS);
}