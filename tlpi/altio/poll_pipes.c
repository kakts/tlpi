/**
 * @file poll_pipes.c
 * 63-2 poll()による複数のファイルディスクリプタの監視
 * 
 * e.g)
 * パイプを十個作成し、そのうち3つをランダムに選択してデータを書き込む
 * ./poll_pipes 10 3
 */
#include <time.h>
#include <poll.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int numPipes, j, ready, randPipe, numWrites;
    int (*pfds)[2];
    struct pollfd *pollFd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr(argv[0]);
    }

    // コマンドラインに与えられたパイプ数に応じた配列を割り当て
    numPipes = getInt(argv[1], GN_GT_0, "num-pipes");

    pfds = calloc(numPipes, sizeof(int[2]));
    if (pfds == NULL) {
        errExit("calloc");
    }
    pollFd = calloc(numPipes, sizeof(struct pollfd));
    if (pollFd == NULL) {
        errExit("calloc");
    }

    // コマンドラインに指定された数のパイプを作成
    for (j = 0; j < numPipes; j++) {
        if (pipe(pfds[j]) == -1) {
            errExit("pipe %d", j);
        }
    }

    // コマンドラインに与えられた数の回数分ランダムに選択したパイプへ書き込む
    numWrites = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-writes") : 1;

    srandom((int) time(NULL));
    for (j = 0; j < numWrites; j++) {
        randPipe = random() % numPipes;
        printf("Writing to fd: %3d (read fd: %3d)\n", pfds[randPipe][1], pfds[randPipe][0]);

        if (write(pfds[randPipe][1], "a", 1) == -1) {
            errExit("write %d", pfds[randPipe][1]);
        }
    }

    /**
     * poll()に指定する配列を生成し、全パイプの読み取り口を監視
     */
    for (j = 0; j < numPipes; j++) {
        pollFd[j].fd = pfds[j][0];
        pollFd[j].events = POLLIN;
    }

    // ノンブロッキングで全パイプの読み取り口を監視
    ready = poll(pollFd, numPipes, 0);
    if (ready == -1) {
        errExit("poll");
    }

    printf("poll() returned: %d\n", ready);

    // どのパイプが読み取り可能になったかを確認
    for (j = 0; j < numPipes; j++) {
        if (pollFd[j].revents & POLLIN) {
            printf("Readable: %3d\n", pollFd[j].fd);
        }
    }
    exit(EXIT_SUCCESS);
}
