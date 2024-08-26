/**
 * @file ud_ucase_cl.c
 * 57-7 UNIXドメインのデータグラムソケットを用いたクライアント
 */
#include "ud_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s msg...\n", argv[0]);
    }

    /**
     * クライアント用のUNIXドメインソケットを作成
     * 一意なパス名(PIDを利用する)にバインドする
     */
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld", (long) getpid());

    if (bind(sfd, (struct sockadder *) &claddr, sizeof(struct sockaddr_un)) == -1) {
        errExit("bind");
    }

    /**
     * サーバアドレスを生成
     */
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    /**
     * サーバへメッセージを送信し、レスポンスをstdoutに出力
     */
    for (j = 1; j < argc; j++) {
        // メッセージはBUF_SIZEより大きい可能性がある
        msgLen = strlen(argv[j]);

        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) != msgLen) {
            fatal("sendto");
        }

        // サーバからのレスポンスを受信
        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if (numBytes == -1) {
            errExit("recvfrom");
        }
        printf("Response %d: %.*s\n", j, (int) numBytes, resp);
    }

    // クライアント用のUNIXドメインソケットパス名を削除
    remove(claddr.sun_path);
    exit(EXIT_SUCCESS);
}
