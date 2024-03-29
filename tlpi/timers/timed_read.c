/**
 * @file timed_read.c
 * 23-2 read()のタイムアウト設定
 * 
 */

#include <signal.h>
#include "../lib/tlpi_hdr.h"

#define BUF_SIZE 200

// SIGALRMハンドラ
static void handler(int sig)
{
    printf("Caught signal\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t numRead;

    int savedErrno;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [num-secs [restart-flag]]\n", argv[0]);
    }
    
    /**
     * SIGALRMのハンドラを設定する
     * コマンドライン引数が与えられなければ、システムコールを自動再開しない
     */
    sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    // readの実行前にalarmを設定する
    alarm((argc > 1) ? getInt(argv[1], GN_NONNEG, "num-secs") : 10);

    numRead = read(STDIN_FILENO, buf, BUF_SIZE - 1);

    // alarm()のタイムアウトによってread()が中断された場合、errnoはEINTRに設定されるので、これを退避させる
    savedErrno = errno;
    // 明示的にタイマー停止
    alarm(0);
    errno = savedErrno;

    // read()の結果
    if (numRead == -1) {
        // readの終了
        if (errno == EINTR) {
            printf("Read timed out\n");
        } else {
            errMsg("read");
        }
    } else {
        printf("Successful read (%ld bytes): %.*s", (long) numRead, (int) numRead, buf);
    }

    exit(EXIT_SUCCESS);
}