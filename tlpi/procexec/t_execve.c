/**
 * @file t_execve.c
 * 27-1 execve()の使用例
 * 
 * 新規プログラム用にパラメータリストと環境を作成し、execve()を実行
 */
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    char *argVec[10];   // 新規プログラムの引数リスト 要素数は多めに設定しておく
    char *envVec[] = {"GREET=salut", "BYE=adieu", NULL};

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s pathname\n", argv[0]);
    }

    // argv[1]のbasenameを得る
    // strrchr：指定した文字が文字列中に最後にあらわれる位置のポインタを返す
    argVec[0] = strrchr(argv[1], '/');
    // 
    if (argVec[0] != NULL) {
        // / が見つかったら、次の文字を指すようにする
        argVec[0]++;
    } else {
        // 見つからない場合は、argv[1]の先頭を指すようにする
        argVec[0] = (char *) argv[1];
    }

    // 新規プログラムの引数リストを作成
    argVec[1] = "hello world";
    argVec[2] = "goodbye";
    argVec[3] = NULL; // リストはNULLで終了させる


    printf("t_execve: Before calling execve %s\n", argv[1]);
    execve(argv[1], argVec, envVec);

    // execve()が戻ってきたらエラー
    errExit("execve");
}
