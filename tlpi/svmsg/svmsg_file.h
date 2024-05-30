/**
 * @file svmsg_file.h
 * 46-7 svmsg_file server client共通ヘッダファイル
 */
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h> // offsetof()の定義
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

#define SERVER_KEY 0x1aaaaaa1 // サーバのメッセージキューのキー

/**
 * リクエストメッセージのデータ構造
 */
struct requestMsg {
    long mtype;
    int clientId;
    char pathname[PATH_MAX];
};

/**
 * REQ_MSG_SIZEは requestMsg構造体のmtextのサイズを計算します
 * clientIdとpathnameフィールドの間にパディングバイトがある場合を考慮し、offsetof()を利用して計算します
 */
#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - offsetof(struct requestMsg, clientId) + PATH_MAX)

#define RESP_MSG_SIZE 8192

/**
 * レスポンスメッセージのデータ構造
 */
struct responseMsg {
    long mtype;
    char data[RESP_MSG_SIZE];
};


// サーバからクライアントへのメッセージのtype
#define RESP_MT_FAILURE 1 // ファイルがオープンできない
#define RESP_MT_DATA 2    // ファイルデータを返す
#define RESP_MT_END 3     // ファイルデータの終わりを示す