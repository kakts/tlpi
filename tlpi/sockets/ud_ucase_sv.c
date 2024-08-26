/**
 * @file ud_ucase_sv.c
 * 57-6 UNIXドメインのデータグラムソケットを用いたサーバ
 */
#include "ud_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    // UNIXドメインのデータグラムソケットを作成
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    /**
     * well known アドレスを生成し、バインド
     */
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        errExit("remove-%s", SV_SOCK_PATH);
    }

    // sockaddr_un構造体の初期化 0で埋める
    memset(&svaddr, 0, sizeof(struct sockaddr_un));

    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
        errExit("bind");
    }

    /**
     * メッセージを受信し、大文字へ変換し、クライアントへ送信
     */
    for (;;) {
        len = sizeof(struct sockaddr_un);

        // クライアントからのデータグラムを受信
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
        if (numBytes == -1) {
            errExit("recvfrom");
        }

        printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

        // 受信したデータを大文字に変換
        for (j = 0; j < numBytes; j++) {
            buf[j] = toupper((unsigned char) buf[j]);
        }

        // クライアントへ送信
        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
            fatal("sendto");
        }
    }
}
