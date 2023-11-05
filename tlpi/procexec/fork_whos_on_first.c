/**
 * @file fork_whos_on_first.c
 * 24-4 fork()実行後、親子プロセスのどちらかが先に実行されるかを確認する例
 * 基本的には非決定動作となるので、決まってはいない
 */
#include <sys/wait.h>
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int numChildren, j;
    pid_t childPid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [num-children]\n", argv[0]);
    }

    // 生成する子プロセスの数を取得
    numChildren = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;

    for (j = 0; j < numChildren; j++) {
        switch (childPid = fork()) {
        case -1:
            errExit("fork");
        case 0:
            printf("%d child\n", j);
            _exit(EXIT_SUCCESS);
        default:
            // 親プロセス 子プロセスの終了を待つ
            printf("%d parent\n", j);
            wait(NULL);
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
