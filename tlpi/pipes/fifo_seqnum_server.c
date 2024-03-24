/**
 * @file fifo_seqnum_server.c
 *
 * 44-7 FIFOを用いたサンプルサーバ エコーサーバ
 * 
 * 利用方法
 * 1. サーバを起動 バックグラウンドで実行　
 *  $ ./fifo_seqnum_server &
 * 2. クライアントを起動
 * $ ./fifo_seqnum_client 3 シーケンス番号を3個要求
 * $ ./fifo_seqnum_client 2 シーケンス番号を2個要求
 */
#include <signal.h>
#include "fifo_seqnum.h"


int main(int argc, char *argv[])
{
    int serverFd, dummyFd, clientFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;

    // サーバのサービス内容本体
    int seqNum = 0;

    // well-known FIFOを作成し、読み取り用にオープン
    // プロセスのファイル作成マスクを設定
    // 引数0: 全ての読み取り、書き込み、実行を許可(777)
    umask(0);

    // FIFOの作成
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        errExit("mkfifo %s", SERVER_FIFO);
    }

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1) {
        errExit("open %s", SERVER_FIFO);
    }
    if (serverFd == -1) {
        errExit("open %s", SERVER_FIFO);
    }


    // EOFが発生しないように、FIFOに対して書き込みようにもオープン
    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (dummyFd == -1) {
        errExit("open %s", SERVER_FIFO);
    }

    // SIGPIPEシグナルを無視する
    // SIGPIPEシグナルは、プロセスが閉じられたパイプに書き込もうとした時に発生するシウgなる
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        errExit("signal");
    }

    // リクエストを受信し、レスポンスを送信
    for (;;) {
        // サーバ用のFIFOからリクエストを読み込む
        if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }

        // クライアントFIFOのオープン (クライアント側で作成済み)

        // クライアントFIFOの名前を作成
        // 指定されたテンプレートを使用して、クライアントFIFOの名前を作成
        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) req.pid);

        // クライアントFIFOをオープン
        clientFd = open(clientFifo, O_WRONLY);
        if (clientFd == -1) {
            errMsg("open %s", clientFifo);
            continue;
        }

        // レスポンスを送信し、FIFOをクローズ
        resp.seqNum = seqNum;
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
            fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
        }

        if (close(clientFd) == -1) {
            errMsg("close");
        }

        seqNum += req.seqLen;
        

    }
}
