/**
 * @file t_kill.c
 * 20-3 kill()を使用したシグナルの送信
 */
#include <signal.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int s, sig;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s pid sig-num\n", argv[0]);
    }

    sig = getInt(argv[2], 0, "sig-num");

    // シグナルの送信
    s = kill(getLong(argv[1], 0, "pid"), sig);

    if (sig != 0) {
        if (s == -1) {
            errExit("kill");
        }
    } else {
        // nullシグナル sig==0 プロセスの存在確認を行う
        if (s == 0) {
            // 成功時
            printf("Process exists and we can send it a signal\n");
        } else {
            if (errno == EPERM) {
                // プロセスは存在するが、シグナルを送信する権限がない
                printf("Process exists, but we don't have permission to send it a signal\n");
            } else if (errno == ESRCH) {
                // プロセスが存在しない
                printf("Process does not exist\n");
            } else {
                // その他のエラー
                errExit("kill");
            }
        }

    }

    exit(EXIT_SUCCESS);
}