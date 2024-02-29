/**
 * @file simple_pipe.c
 * 
 * 44-2 パイプによる親子プロセス間通信
 * 
 */

#include <sys/wait.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
    // パイプのファイルディスクリプタ
    int pfd[2];

    char buf[BUF_SIZE];
    ssize_t numRead;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s string\n", argv[0]);
    }

    // パイプ作成
    // pipe()の実行により、引数に渡した配列に2つのファイルディスクリプタが格納される
    // 0: パイプの読み取り口
    // 1: パイプの書き込み口
    if (pipe(pfd) == -1) {
        errExit("pipe");
    }

    // forkする
    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        // 子プロセス パイプを読み取る
        // 読み取り専用のため、書き込み用のfdはcloseする
        if (close(pfd[1]) == -1) {
            errExit("close - child");
        }

        // パイプを読み取り、stdoutへ出力する
        for (;;) {
            numRead = read(pfd[0], buf, BUF_SIZE);
            if (numRead == -1) {
                errExit("read");
            } else if (numRead == 0) {
                // EOF
                break;
            } else if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                // stdoutへの書き込み失敗
                fatal("child - partial/failed write");
            }
        }

        // 改行出力
        write(STDOUT_FILENO, "\n", 1);
        // 読み取り終了したので、読み取り口をclose
        if (close(pfd[0]) == -1) {
            errExit("close");
        }
        _exit(EXIT_SUCCESS);

    default:
        // 親プロセス パイプへ書き込む
        // 読み取り口は不要なためclose
        if (close(pfd[0]) == -1) {
            errExit("close - parent");
        }

        // パイプへ書き込む
        if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
            fatal("parent - partial/failed write");
        }

        // 書き込み終了したため、close
        if (close(pfd[1]) == -1) {
            errExit("close");
        }

        // 子プロセスの終了を待つ
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}