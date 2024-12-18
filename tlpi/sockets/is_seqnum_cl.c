/**
 * @file is_seqnum_cl.c
 * 59-7 ストリームソケットを用いたクライアントプログラム
 */
#include <netdb.h>
#include "is_seqnum.h"

int main(int argc, char *argv[])
{
    // リクエストするシーケンス番号個数
    char *reqLenStr;
    // 返された先頭シーケンス番号
    char seqNumStr[INT_LEN];

    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    }

    /**
     * 接続先ソケットアドレス構造体をgetaddrinfo()で取得
     */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; // IPv4 と IPv6両方を受け入れる
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_NUMERICSERV; // serviceを数字で表現したポート番号文字列と解釈する

    if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0) {
        errExit("getaddrinfo");
    }

    /**
     * 接続可能なアドレス構造体に出会うまでリストを探索
     */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1) {
            continue;
        }

        // サーバに接続
        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }

        // 接続に失敗した場合はソケットをクローズして次のアドレス構造体を試す
        close(cfd);
    }

    if (rp == NULL) {
        fatal("Could not connect socket to any address");
    }

    freeaddrinfo(result);

    /**
     * リクエストするシーケンス番号個数を改行文字で終端する文字列として送信
     */
    reqLenStr = (argc > 2) ? argv[2] : "1";
    if (write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr)) {
        fatal("Partial/failed write (reqLenStr)");
    }
    if (write(cfd, "\n", 1) != 1) {
        fatal("Partial/failed write (newline)");
    }


    /**
     * サーバからのレスポンスデータ(シーケンス番号)を読み取り、表示
     */
    numRead = readLine(cfd, seqNumStr, INT_LEN);
    if (numRead == -1) {
        errExit("readLine");
    }
    if (numRead == 0) {
        fatal("Unexpected EOF from server");
    }

    printf("Sequence number: %s", seqNumStr);
    exit(EXIT_SUCCESS);
}
