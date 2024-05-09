/**
 * @file svmsg_send.c
 * 46-2 System V メッセージキュー msgsnd()によるキューの送信
 */
#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"

#define MAX_MTEXT 1024

/**
 * キューのメッセージ用の構造体
 */
struct mbuf {
    long mtype; // メッセージ種類
    char mtext[MAX_MTEXT]; // メッセージ本文
};

static void usageError(const char *progName, const char *msg)
{
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }
    fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
    fprintf(stderr, "    -n           Use IPC_NOWAIT flag\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int msgid, flags, msgLen;
    struct mbuf msg; // msgsnd()用のメッセージバッファ
    int opt; // getopt()用のオプション文字

    // コマンドライン・パラメータの解析
    flags = 0;
    while ((opt = getopt(argc, argv, "n")) != -1) {
        if (opt == 'n') {
            // msgflgにIPC_NOWAITフラグを設定
            flags |= IPC_NOWAIT;
        } else {
            usageError(argv[0], NULL);
        }
    }
    // optindは次に処理するコマンドライン引数のインデックスを表す
    if (argc < optind + 2 || argc > optind + 3) {
        usageError(argv[0], "Wrong number of arguments\n");
    }

    msgid = getInt(argv[optind], 0, "msqid");
    msg.mtype = getInt(argv[optind + 1], 0, "msg-type");

    if (argc > optind + 2) {
        // メッセージ本文が指定されている場合
        msgLen = strlen(argv[optind + 2]) + 1;
        if (msgLen > MAX_MTEXT) {
            cmdLineErr("msg-text too long (max: %d characters)\n", MAX_MTEXT);
        }

        // メッセージ本文をコピー
        memcpy(msg.mtext, argv[optind + 2], msgLen);
    } else {
        msgLen = 0;
    }

    // メッセージの送信
    if (msgsnd(msgid, &msg, msgLen, flags) == -1) {
        errExit("msgsnd");
    }

    exit(EXIT_SUCCESS);
}
