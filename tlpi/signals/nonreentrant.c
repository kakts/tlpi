/**
 * @file nonreentrant.c
 * 21-1 main()とシグナルハンドラの双方でnon-reentrant関数を呼び出す
 */

// 600以上の値で定義するとSUSv3関連の定義とC99での定義が追加で公開される
#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600
#endif

#define _GNU_SOURCE
#include <crypt.h>
#include <signal.h>
#include <string.h>
#include "../lib/tlpi_hdr.h"

// argv[2]の文字列
static char *str2;

// ハンドラ実行回数
static int handled = 0;

struct crypt_data data;

/**
 * シグナルハンドラ
 * str2 をcrypt()で暗号化する
 */
static void handler(int sig)
{
    crypt(str2, "xx");
    handled++;
}

int main(int argc, char *argv[])
{
    char *cr1;

    int callNum, mismatch;
    struct sigaction sa;

    // 引数チェック
    if (argc != 3) {
        usageErr("%s str1 str2\n", argv[0]);
    }

    // argv[2]をstatic変数に代入し、ハンドラから使用可能にする
    str2 = argv[2];
    
    // staticに割り当てられた文字列を別メモリへコピー
    cr1 = strdup(crypt(argv[1], "xx"));

    if (cr1 == NULL) {
        errExit("strdup");
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    // argv[1]を繰り返しcrypt()で暗号化する。
    // シグナルハンドラが割り込むと、crypt()内部のstatic memoryがargv[2]の暗号化結果に上書きされ、
    // strcmp()がcr1との不一致を検出する

    for (callNum = 1, mismatch = 0; ; callNum++) {
        
        // すでにargv[1]を暗号化した文字列cr1 と 再度argv[1]を暗号化した文字列を比較する
        if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
            mismatch++;
            printf("Mismatch on call %d (mismatch=%d handled=%d)\n", callNum, mismatch, handled);
        }
    }
}