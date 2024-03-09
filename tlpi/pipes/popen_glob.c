/**
 * @file popen_glob.c
 * 
 * 44-5 popen() pclose()を用いたファイル名パターン展開
 * 
 */
#include <ctype.h>
#include <limits.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define POPEN_FMT "/bin/ls -d %s 2> /dev/null"
#define PAT_SIZE 50
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)

int main(int argc, char *argv[])
{
    // 展開するパターン
    char pat[PAT_SIZE];
    // popen()で実行するコマンド
    char popenCmd[PCMD_BUF_SIZE];
    // popen()が返すファイルストリーム
    FILE *fp;

    // 'pat' は無効な文字を含むかどうか
    Boolean badPattern;

    int len, status, fileCnt, j;
    char pathname[PATH_MAX];

    // パターンを読み取り展開結果を表示
    for (;;) {
        printf("pattern: ");
        fflush(stdout);
        // stdinから文字列を読み取り、patという名前の配列に格納
        
        if (fgets(pat, PAT_SIZE, stdin) == NULL) {
            // EOF
            break;
        }

        printf("%s\n", pat);
        len = strlen(pat);
        if (len <= 1) {
            // 空行
            continue;
        }

        // 末尾の改行を削除        
        if (pat[len - 1] == '\n') {
            pat[len - 1] = '\0';
        }

        /**
         * パターンに無効な文字がないことを確認
         * 有効なのは文字・数字・アンダースコア・ドット・シェルが解釈するワイルドカード
         */
        for (j = 0, badPattern = FALSE; j < len && !badPattern; j++) {
            printf("pat[j]: %c\n", pat[j]);
            if (!isalnum((unsigned char) pat[j]) && strchr("_*?[^-].", pat[j]) == NULL) {
                printf("BadPatern true pat[j]: %c\n", pat[j]);
                badPattern = TRUE;
            }
        }

        if (badPattern) {
            printf("Bad pattern character: %c\n", pat[j - 1]);
            continue;
        }

        // 'pat'を解釈するコマンド文字列を生成し実行
        snprintf(popenCmd, PCMD_BUF_SIZE, POPEN_FMT, pat);

        // 文字列がNULLで終端することを保証
        fp = popen(popenCmd, "r");
        if (fp == NULL) {
            printf("popen() failed\n");
            continue;
        }

        // EOFに達するまでパス名の一覧を読み取る
        fileCnt = 0;
        while (fgets(pathname, PATH_MAX, fp) != NULL) {
            printf("%s", pathname);
            fileCnt++;
        }

        // パイプをクローズし、終了ステータスを取得し表示
        status = pclose(fp);
        printf("    %d matching files\n", fileCnt, (fileCnt != 1) ? "s" : "");
        printf("    pclose() status == %#x\n", (unsigned int) status);
        if (status != -1) {
            printWaitStatus("\t", status);
        }
    }

    exit(EXIT_SUCCESS);
}
