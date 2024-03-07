/**
 * @file pipe_ls_wc.c
 *
 * 44-4 パイプによるlsとwcの連結
 * 
 * 例
 * ./pipe_ls_wc
 */

#include <sys/wait.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    // パイプのファイルディスクリプタ
    int pfd[2];

    if (pipe(pfd) == -1) {
        errExit("pipe");
    }

    switch (fork()) {
    case -1:   
        errExit("fork");
    case 0:
        // 子プロセス 'ls' を実行しパイプへ書き込む
        if (close(pfd[0]) == -1) {
            // 読み取り口は不要
            errExit("close 1");
        }
        
        // stdoutを書き込み口に複製する。　元のfdはクローズする
        if (pfd[1] != STDOUT_FILENO) {
            if (dup2(pfd[1], STDOUT_FILENO) == -1) {
                errExit("dup2 1");
            }
            // 元の書き込み口をクローズする
            if (close(pfd[1]) == -1) {
                errExit("close 2");
            }
        }

        // lsを実行し、パイプへ書き込む
        execlp("ls", "ls", (char *) NULL);
        errExit("execlp ls");
    default:
        //親プロセスは次の子プロセス作成
        break;
    }

    // wc用の子プロセス作成
    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        // 子プロセス 'wc'を実行しパイプを読み取る

        // 書き込み口は不要
        if (close(pfd[1]) == -1) {
            errExit("close 3");
        }

        // stdinを読み取り口に複製する 元のfdはクローズ
        if (pfd[0] != STDIN_FILENO) {
            if (dup2(pfd[0], STDIN_FILENO) == -1) {
                errExit("dup2 2");
            }
            if (close(pfd[0]) == -1) {
                errExit("close 4");
            }
        }
        execlp("wc", "wc", "-l", (char *) NULL);
        errExit("execlp wc");
    default:
        break;
    }

    // 親プロセスはパイプを使用しないためクローズし、2つの子プロセスの終了を待つ
    if (close(pfd[0]) == -1) {
        errExit("close 5");
    }
    if (close(pfd[1]) == -1) {
        errExit("close 6");
    }

    if (wait(NULL) == -1) {
        errExit("wait 1");
    }
    if (wait(NULL) == -1) {
        errExit("wait 2");
    }

    exit(EXIT_SUCCESS);
}
