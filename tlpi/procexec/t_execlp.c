/**
 * @file t_execlp.c
 * 27-3 execlp()によるPATH検索
 */

#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s pathname\n", argv[0]);
    }

    // 第1引数はプログラムのパスを指定
    // 第2引数はプログラム自体の名前を指定
    // 第2引数にechoを渡した場合
    // echo "hello world"を実行することを意味する
    execlp(argv[1], argv[1], "hello world", (char *) NULL);
    errExit("execlp");
}
