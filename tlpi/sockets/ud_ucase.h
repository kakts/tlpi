/**
 * @file ud_ucase.h
 * 57-5 UNIXドメインのデータグラムソケットを用いたサーバとクライアント用のヘッダファイル
 */
#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

/**
 * クライアント・サーバ間で通信するメッセージの最大サイズ
 */
#define BUF_SIZE 10

#define SV_SOCK_PATH "/tmp/ud_ucase"
