/**
 * @file footprint.c
 * 
 * 24-3 fork()後の親子プロセスのメモリ使用量を確認する例
 * fork()+wait()を利用する
 * 
 * このコードでは、大量のメモリを消費する関数を含んでいます。
 * プロセスのメモリ使用量の変更を防ぐためには、プログラムはこの関数を実行する子プロセスを生成します。
 * 子プロセスの終了後、すべてのメモリが解放され、親プロセスのメモリ使用量が変更されないことを確認します。
 */
#define _BSD_SOURCE
#include <sys/wait.h>
#include "tlpi_hdr.h"

/**
 * 子プロセスで実行される関数
 * 大量のメモリを消費する
 */
static int func(int arg)
{
    int j;

    for (j = 0; j < 0x100; j++) {
        // 0x8000 8MB、つまり32KBのメモリを確保する
        if (malloc(0x8000) == NULL) {
            errExit("malloc");
        }
    }

    // 実行中のプログラムのヒープ領域の終端位置を表示
    printf("Program break in child:  %10p\n", sbrk(0));
    return arg;
}

int main(int argc, char const *argv[])
{
    int arg = (argc > 1) ? getInt(argv[1], 0, "arg") : 0;

    pid_t childPid;
    int status;

    // stdoutのバッファリング停止
    setbuf(stdout, NULL);

    printf("Program break in parent: %10p\n", sbrk(0));

    childPid = fork();
    if (childPid == -1) {
        errExit("fork");
    }
    if (childPid == 0) {
        // 子プロセス: func()を実行し、その戻り値を終了コードとして終了する
        exit(func(arg));
    }

    // 親プロセス: 子プロセスの終了を待つ func()の戻り値はstatusから得られる
    if (wait(&status) == -1) {
        errExit("wait");
    }

    // 実行中のプログラムのヒープ領域の終端位置を表示
    printf("Program break in parent: %10p\n", sbrk(0));
    printf("Status = %d %d\n", status, WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}