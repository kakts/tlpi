/**
 * @file i6d_ucase_cl.c
 * 59-4 IPv6 データグラムソケットを用いた大文字変換クライアント
 */
#include "i6d_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s host-address msg...\n", argv[0]);
    }

    // IPv6のデータグラムソケットを作成
    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    // サーバのアドレス構造体を初期化
    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    // サーバのポート番号 をホストバイトオーダーからネットワークバイトオーダーに変換
    svaddr.sin6_port = htons(PORT_NUM);
    if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0) {
        fatal("inet_pton failed for address '%s'", argv[1]);
    }

    // サーバへメッセージ送信・レスポンスメッセージをstdoutへ出力
    for (j = 2; j < argc; j++) {
        msgLen = strlen(argv[j]);
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msgLen) {
            fatal("sendto");
        }

        // サーバからのレスポンスを受信
        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if (numBytes == -1) {
            errExit("recvfrom");
        }

        printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}
