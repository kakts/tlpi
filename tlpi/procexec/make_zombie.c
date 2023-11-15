/**
 * @file make_zombie.c
 * 26-4
 * 
 * プロセスをゾンビ化し、ゾンビプロセスはSIGKILLでも消滅させられないことを示す
 * プログラム実行により、 ps の内容を確認し、ゾンビ状態を表す <defunct>が表示されることを確認する
 */

#include <signal.h>
#include <libgen.h>
#include "tlpi_hdr.h"

#define CMD_SIZE 200

int main(int argc, char const *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;

    // stdoutのバッファ停止
    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0: // 子プロセス すぐに終了し、ゾンビプロセスとなる
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        _exit(EXIT_SUCCESS);
    default: // 親プロセス 子プロセスが開始、終了する時間を稼ぐ
        sleep(3);

        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));

        // 文字列はNULLで終了
        // 子プロセスのゾンビ化を確認
        system(cmd);

        // ゾンビプロセスを削除するシグナルを送信
        if (kill(childPid, SIGKILL) == -1) {
            errMsg("kill");
        }
        // 子プロセスがシグナルに対応する時間を稼ぐ
        sleep(3);

        printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);

        // 子プロセスのゾンビ化を再確認
        system(cmd);

        exit(EXIT_SUCCESS);

    }
}

