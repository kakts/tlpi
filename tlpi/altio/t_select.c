/**
 * @file t_select.c
 * 63-1 select()による複数のファイルディスクリプタの監視
 * 
 * ./t_select 10 0r
 */
#include <sys/time.h>
#include <sys/select.h>
#include "tlpi_hdr.h"

static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s {timeout|-} fd-num[rw]...\n", progName);
    fprintf(stderr, "    - means infinite timeout; \n");
    fprintf(stderr, "    r = monitor for read\n");
    fprintf(stderr, "    w = monitor for write\n\n");
    fprintf(stderr, "    e.g.: %s - 0rw 1w\n", progName);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    fd_set readfds, writefds;
    int ready, nfds, fd, numRead, j;
    struct timeval timeout;
    struct timeval *pto;
    char buf[10];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageError(argv[0]);
    }

    // argv[1]はselect()のタイムアウト値を指定する
    if (strcmp(argv[1], "-") == 0) {
        pto = NULL; // タイムアウトなし
    } else {
        pto = &timeout;
        timeout.tv_sec = getLong(argv[1], 0, "timeout");
        timeout.tv_usec = 0; // マイクロ秒は0
    }

    // 以降のコマンドラインパラメータからファイルディスクリプタセットを生成
    nfds = 0;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    for (j = 2; j < argc; j++) {
        numRead = sscanf(argv[j], "%d%2[rw]", &fd, buf);
        if (numRead != 2) {
            usageError(argv[0]);
        }

        // ファイルディスクリプタの最大値を超えて設定されている場合
        if (fd >= FD_SETSIZE) {
            cmdLineErr("file descriptor exceeds limit (%d)\n", FD_SETSIZE);
        }

        if (fd >= nfds) {
            nfds = fd + 1;
        }

        // readfds
        if (strchr(buf, 'r') != NULL) {
            FD_SET(fd, &readfds);
        }
        // writefds
        if (strchr(buf, 'w') != NULL) {
            FD_SET(fd, &writefds);
        }
    }

    // コマンドパラメータの処理が終わったので、select()を呼び出す
    ready = select(nfds, &readfds, &writefds, NULL, pto);

    // 例外条件は指定しない
    if (ready == -1) {
        errExit("select");
    }

    // select()が返した結果を表示
    printf("ready = %d\n", ready);
    for (fd = 0; fd < nfds; fd++) {
        printf("%d: %s%s\n", fd, FD_ISSET(fd, &readfds) ? "r" : "",
               FD_ISSET(fd, &writefds) ? "w" : "");
    }

    if (pto != NULL) {
        printf("timeout after select(): %ld.%03ld\n",
               (long)timeout.tv_sec, (long)timeout.tv_usec / 1000);
    }
    exit(EXIT_SUCCESS);
}
