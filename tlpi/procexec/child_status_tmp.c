/**
 * @file child_status.c
 * 26-3 waitpid()により子プロセスの状態を知る
 * 
 * 子プロセスを作成し、子プロセスがコマンド引数に指定された値を終了コードとし、exit()する、もしくはシグナル対応としてpause()をループする
 * 親プロセスは、waitpid()により、子プロセスを監視し、得られたwaitステータスの値を表示し、 printWaitStatus()で内容を表示する
 * 
 * 親プロセスは、子プロセスが正常終了や、シグナルにより終了したのを確認して終了する
 */

#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int status;

    pid_t childPid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [exit-status]\n", argv[0]);
    }

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        // 子プロセス コマンドライン引数で渡された値を終了コードとして終了するか、シグナルを繰り返し待つ
        printf("Child started with PID = %ld\n", (long)getpid());
        if (argc > 1) {
            // コマンドライン引数で終了コードが指定された場合
            exit(getInt(argv[1], 0, "exit-status"));
        } else {
            // シグナルを繰り返し待つ
            for (;;) {
                pause();
            }
        }
        exit(EXIT_FAILURE);
    default:
        // 親プロセス 子プロセスの終了を待つ
        for (;;) {
            childPid = waitpid(-1, &status, WUNTRACED
#ifdef WCONTINUED
                    | WCONTINUED
#endif
                    );
            if (childPid == -1) {
                errExit("waitpid");
            }

            // 子プロセスの状態を16進数と、分割した10進数で表示する
            printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                    (long)childPid,
                    (unsigned int)status, status >> 8, status & 0xff);
            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                exit(EXIT_SUCCESS);
            }
        }
    }
}
