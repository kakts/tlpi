/**
 * @file print_wait_status.c
 * 26-2 wait()ファミリが得るステータスの意味を表示する関数
 */

#define _GNU_SOURCE
#include <string.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

void printWaitStatus(const char *msg, int status)
{
    if (msg != NULL) {
        printf("%s", msg);
    }

    // 子プロセスが終了した場合
    if (WIFEXITED(status)) {
        // WEXITSTATUS()は、子プロセスの終了コードを返す
        printf("child exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        // 子プロセスがシグナルで終了した場合
        printf("child killed by signal %d (%s)",
                WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
        // WCOREDUMP()は、子プロセスがコアダンプを生成した場合に真を返す
        if (WCOREDUMP(status)) {
            printf(" (core dumped)");
        }
#endif
        printf("\n");
    } else if (WIFSTOPPED(status)) {
        // 子プロセスがシグナルで停止した場合
        printf("child stopped by signal %d (%s)\n",
                WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
    } else if (WIFCONTINUED(status)) {
        // 子プロセスがシグナルで停止した後、再開した場合
        printf("child continued\n");
#endif
    } else {
        // 予期しない状態
        printf("what happened to this child? (status=%x)\n",
                (unsigned int)status);
    }
}