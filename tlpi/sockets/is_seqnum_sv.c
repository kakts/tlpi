/**
 * @file is_seqnum_sv.c
 * 59-6 ストリームソケットによりクライアントと通信するエコーサーバ
 */

// #define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include <netdb.h>
#include "is_seqnum.h"

#define BACKLOG 50

int main(int argc, char *argv[])
{
    uint32_t seqNum;
    char reqLenStr[INT_LEN]; // シーケンス番号要求数
    char seqNumStr[INT_LEN]; // サービスするシーケンス番号の先頭

    struct sockaddr_storage claddr;
    int lfd, cfd, optval, reqLen;

    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [init-seq-num]\n", argv[0]);
    }
    
    seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

    // クローズされたソケットへwriteした際に発生する恐れのあるSIGPIPEシグナルを無視し、EPIPEエラーを返すようにする
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        errExit("signal");
    }

    /**
     * バインドするアドレスのリストをgetaddrinfo()で取得する
     */
    // addrinfo構造体のヒントを初期化する
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_family = AF_UNSPEC; // AF_UNSPECの場合 IPv4 と IPv6両方を受け入れる
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // AI_PASSIVE: bind()のためのアドレスを返す. AI_NUMERICSERV: serviceを数字で表現したポート番号文字列と解釈する
    
    // getaddrinfo()でサービス名に対するアドレスリストを取得する
    if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0) {
        errExit("getaddrinfo");
    }

    /**
     * ソケットにバインド可能なアドレス構造体に出会うまでリストを検索
     */
    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (lfd == -1) {
            continue;
        }

        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
            errExit("setsockopt");
        }

        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }

        // bind()に失敗した場合はソケットをクローズして次のアドレス構造体を試す
        close(lfd);
    }

    // どのアドレス構造体にもバインドできなかった場合
    if (rp == NULL) {
        fatal("Could not bind socket to any address");
    }

    // getnameinfo()でバインドしたアドレスをlistenする
    if (listen(lfd, BACKLOG) == -1) {
        errExit("listen");
    }

    freeaddrinfo(result);

    /**
     * クライアントを反復処理
     */
    for (;;) {
        /**
         * 接続要求を受け付け、クライアントのアドレスを取得
         */
        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);

        if (cfd == -1) {
            errMsg("accept");
            continue;
        }

        if (getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
        } else {
            snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        }

        printf("Connection from %s\n", addrStr);

        // クライアントからのリクエストデータを読み取り、シーケンス番号を返す
        if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
            close(cfd);
            continue;
        }

        reqLen = atoi(reqLenStr);
        // 不正なクライアントへの対応
        if (reqLen <= 0) {
            close(cfd);
            continue;
        }

        snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
        if (write(cfd, seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr)) {
            fprintf(stderr, "Error on write");
        }

        // シーケンス番号の更新
        seqNum += reqLen;
        if (close(cfd) == -1) {
            errMsg("close");
        }
    }
}
