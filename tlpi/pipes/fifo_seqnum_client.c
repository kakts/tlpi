/**
 * @file fifo_seqnum_client.c
 *
 * 44-8 シーケンス番号サーバのクライアント
 */

#include "fifo_seqnum.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];

/**
 * 終了時に実行される関数 クライアントFIFOを削除する
 */
static void removeFifo(void)
{
    unlink(clientFifo);
}

 
int main(int argc, char *argv[])
{
    int serverFd, clientFd;

    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [seq-len...]\n", argv[0]);
    }

    // クライアントFIFOの作成
    // 指定通りのパーミッションで作成するため umaskを0に設定
    umask(0);

    // クライアントFIFO名の作成
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) getpid());

    // クライアントFIFOの作成
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST) {
        errExit("mkfifo %s", clientFifo);
    }

    // リクエストメッセージ生成、サーバFIFOオープン、リクエスト送信を行う
    req.pid = getpid();
    req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

    // サーバFIFOをオープン
    serverFd = open(SERVER_FIFO, O_WRONLY);

    if (serverFd == -1) {
        errExit("open %s", SERVER_FIFO);
    }

    // リクエストメッセージを送信
    if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
        fatal("Can't write to server");
    }

    // クライアントFIFOをオープンし、レスポンスを受信・表示
    clientFd = open(clientFifo, O_RDONLY);
    if (clientFd == -1) {
        errExit("open %s", clientFifo);
    }

    // サーバからのレスポンスを読み込む
    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
        fatal("Can't read response from server");
    }

    printf("%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}
