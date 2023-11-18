/**
 * @file t_system.c
 * 27-7 system()の使用例
 */

#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_CMD_LEN 200

int main(int argc, char const *argv[])
{
    // system()が実行するコマンド文字列
    char str[MAX_CMD_LEN];
    int status;

    for (;;) {
        printf("Command: ");
        fflush(stdout);

        // 標準入力からコマンド文字列を読み込む
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }

        // 指定したコマンドをsystem()により実行
        status = system(str);
        printf("system() returned: status=0x%04x (%d,%d)\n",
                (unsigned int) status, status >> 8, status & 0xff);

        if (status == -1) {
            errExit("system");
        }

        // system()によってコマンド実行のために作成された子プロセスがシェルを実行できない場合
        // 127が返される
        if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
            printf("(Probably) could not invoke shell\n");
        } else {
            // コマンド実行成功
            printWaitStatus(NULL, status);
        }
    }
    exit(EXIT_SUCCESS);
}
