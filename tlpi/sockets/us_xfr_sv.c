/**
 * @file us_xfr_sv.c
 * 57-3
 * UNIXドメインのストリームソケットを用いたシンプルサーバ
 */
#include "us_xfr.h"

#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // UNIXドメインのストリームソケットを作成
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    /**
     * サーバソケットアドレスの生成、バインド、listen開始
     */

    // 既存のソケットファイルの削除を行い、削除に失敗し、ファイルが存在しないエラー以外の場合
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        errExit("remove-%s", SV_SOCK_PATH);
    }

    // sockaddr_un構造体の初期化 0で埋める
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    // ソケットファイルのパスをaddr.sun_pathにコピーする。
    // これによりソケットアドレスがUNIXドメインソケット用に適切に設定される
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // ソケットアドレスをソケットにバインド
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        errExit("bind");
    }

    // ソケットを接続待ち状態にする
    if (listen(sfd, BACKLOG) == -1) {
        errExit("listen");
    }

    /**
     * クライアントからの接続を逐次処理する
     */
    for (;;) {
        /**
         * クライアントからの接続をうけつける。コネクションは新規ソケット'cfd'として返される
         * リスニングソケット'sfd'はオープンしたまま、以降の接続を受け付ける
         */
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            errExit("accept");
        }

        /**
         * 接続済みソケットから受信したデータをEOFに到達するまでstdoutへ転送する
         */
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                fatal("partial/failed write");
            }
        }

        if (numRead == -1) {
            errExit("read");
        }

        if (close(cfd) == -1) {
            errMsg("close");
        }


    }


}
