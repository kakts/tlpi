/**
 * @file inet_sockets.h
 * 59-8 インターネットドメインのソケットを作成するためのヘルパー関数
 */
#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>
#include "tlpi_hdr.h"

/**
 * typeに指定されたソケットを作成し、host, serviceに指定されたアドレスへ接続
 * 
 * @return: 作成されたソケットのファイルディスクリプタ
 */
int inetConnect(const char *host, const char *service, int type);

/**
 * ストリームソケットを作成し、SOCK_STREAM, serviceに指定されたTCPポートのワイルドカードアドレスへバインドし
 * リスニングソケットとする
 * 
 * @return: 作成されたソケットのファイルディスクリプタ
 */
int inetListen(const char *service, int backlog, socklen_t *addrlen);

/**
 * typeに指定されたソケットを作成し、serviceに指定されたポート番号でバインド
 * UDPサーバ・クライアント用
 */
int inetBind(const char *service, int type, socklen_t *addrlen);

/**
 * インターネットソケットアドレスを可読形式へ変換する
 */
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen);

// inetAddressStr()に指定する文字列のサイズ (NI_MAXHOST + NI_MAXSERV + 4)よりも大きくする必要がある
#define IS_ADDR_STR_LEN 4096

#endif
