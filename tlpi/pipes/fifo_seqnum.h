/**
 * @file fifo_seqnum.h
 * 
 * 44-6 fifo_seqnum_server.c , fifo_seqnum_client.c共通のヘッダファイル
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// サーバーが作成するFIFOのパス
#define SERVER_FIFO "/tmp/seqnum_sv"

// クライアント用のFIFO名のテンプレート
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"

// クライアント用のFIFOのパス名の最大長
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

// client->serverへのリクエスト
struct request {
    pid_t pid;      // クライアントのPID
    int seqLen;     // シーケンス番号の長さ
};

// server->clientへの応答
struct response {
    int seqNum;     // シーケンス番号
};
