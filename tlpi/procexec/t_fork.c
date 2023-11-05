/**
 * @file t_fork.c
 * 
 * 24-1 fork()の例
 */

#include "../lib/tlpi_hdr.h"

// データセグメントに割り当てられる変数
static int idata = 111;

int main(int argc, char *argv[])
{
    // スタックセグメントに割り当てられる変数
    int istack = 222;

    pid_t childPid;


    // fork実行
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0: // 子プロセスによって実行される
        idata *= 3;
        istack *= 3;
        break;
    default:
        // 親プロセスによって実行される

        // 本来はfork()後、親子どちらが先に実行されるかは不定
        // 本来は動作は保証されないが、ここでは子プロセスを先に実行するようにsleep()を挟んでいる
        sleep(3);
        break;
    }

    /**
     * 親子プロセスともにここに達する
     */
    printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(), (childPid == 0) ? "(child) " : "(parent)", idata, istack);
    exit(EXIT_SUCCESS);
}