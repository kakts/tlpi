/**
 * @file rlimit_nproc.c
 * 36-3 リソース消費制限RLIMIT_NPROCの設定
 * setrlimit()を使用して、ユーザが作成可能なプロセス数(RLIMIT_NPROC)にソフトリミット/ハードリミットを設定する
 * 
 * 実行例
 * ユーザが作成可能なプロセス数のソフトリミットを30、ハードリミットに100を設定する場合
 * ./rlimit_nproc 30 100
 * 
 * 注意: このプログラムを10個するユーザは非特権ユーザである必要がある
 * RLIMIT＿NPROCのリミットはユーザごとに適用される。 しかし、rootユーザなどの特権ユーザはこの制限を無視できる。
 * そのため、rootユーザとしてこのプログラムを実行すると、RLIMIT_NPROCの設定は無視される。
 */
#include <sys/resource.h>
#include "print_rlimit.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct rlimit rl;
    int j;
    pid_t childPid;

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s soft-limit [hard-limit]\n", argv[0]);
    }

    printRlimit("Initial maximum process limits: ", RLIMIT_NPROC);
    
    // リソース消費制限の設定
    // 未指定の場合、ハードリミットとソフトリミットは同じ値にセットする
    rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : getInt(argv[1], 0, "soft-limit");
    rl.rlim_max = (argc == 2) ? rl.rlim_cur :
                    (argv[2][0] == 'i') ? RLIM_INFINITY : getInt(argv[2], 0, "hard-limit");
    

    // リソース消費制限の設定
    // RLIMIT_NPROC: ユーザが作成可能なプロセスの数
    // ここではユーザが作成可能なプロセス数を、プログラム実行時に指定した値に設定する
    if (setrlimit(RLIMIT_NPROC, &rl) == -1) {
        errExit("setrlimit");
    }

    printRlimit("New maximum process limits:     ", RLIMIT_NPROC);

    // エラーになるまで子プロセスを作成し、RLIMIT_NPROCの設定が有効か確認する
    for (j = 1; ; j++) {
        switch (childPid = fork()) {
        case -1: // fork()エラー
            errExit("fork");
        case 0: // 子プロセス
            _exit(EXIT_SUCCESS);
        default:
            // 親プロセス: 子プロセスのIDを表示する。ゾンビ化したプロセスはそのまま溜め込む
            printf("Child %d (PID=%ld) started\n", j, (long) childPid);
            break;
        }
    }
}
