/**
 * @file i6d_ucase_sv.c
 * 59-3 IPv6 データグラムソケットを用いた大文字変換サーバ
 * 
 * inet_ntop()により recvfrom()で取得したクライアントのホストアドレスを可読形式へ変換する
 */
#include "i6d_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr,  claddr;
    int sfd, j;
    ssize_t numBytes;

    socklent_t len;
    char buf[BUF_SIZE];
    char claddrStr[INET6_ADDRSTRLEN];

    // ipv6のデータグラムソケットを作成
    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    // ソケットアドレス構造体を初期化
    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any; // すべてのアドレスにバインド
    svaddr.sin6_port = htons(PORT_NUM);

    if (bind(sfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in6)) == -1) {
        errExit("bind");
    }

    // メッセージの受信・大文字変換・クライアントへの送信
    for (;;) {
        len = sizeof(struct sockaddr_in6);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

        if (numBytes == -1) {
            errExit("recvfrom");
        }

        if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN) == NULL) {
            printf("Couldn't convert client address to string\n");
        } else {
            printf("Server received %ld bytes from (%s, %u)\n", (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
        }

        for (j = 0; j < numBytes; j++) {
            buf[j] = toupper((unsigned char) buf[j]);
        }

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
            fatal("sendto");
        }
    }
}
