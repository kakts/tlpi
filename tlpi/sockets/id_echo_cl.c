/**
 * @file id_echo_cl.c
 * 60-3 UDP echoクライアント
 */

#include "id_echo.h"

int main(int argc, char *argv[])
{
    int sfd, j;
    size_t len;
    ssize_t numRead;

    char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s host msg...\n", argv[0]);
    }

    // コマンドラインパラメータに与えられたサーバのアドレスを作成
    sfd = inetConnect(argv[1], SERVICE, SOCK_DGRAM);
    if (sfd == -1) {
        errExit("inetConnect");
    }

    // 以降のコマンドラインパラメータを個別のデータグラムとしてサーバへ送信
    for (j = 2; j < argc; j++) {
        len = strlen(argv[j]);
        if (write(sfd, argv[j], len) != len) {
            fatal("partial/failed write");
        }

        numRead = read(sfd, buf, BUF_SIZE);
        if (numRead == -1) {
            errExit("read");
        }

        printf("[%ld bytes] %.*s\n", (long) numRead, (int) numRead, buf);
    }

    exit(EXIT_SUCCESS);
}
