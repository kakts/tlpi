/**
 * @file multi_wait.c
 * 26-1 複数の子プロセスの作成と監視
 * 
 * 引数で指定された秒数だけスリープする子プロセスを作成する。
 * 例 3つの子プロセスで、それぞれ 1, 2, 3秒スリープする場合
 * $ ./multi_wait 1 2 3
 */
#include <sys/wait.h>
#include <time.h>
#include "curr_time.h"
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    // 現在までwait()した子プロセスの数
    int numDead;
    // wait()した子プロセスのID
    pid_t childPid;
    int j;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time...\n", argv[0]);
    
    // stdoutへのバッファリング停止
    setbuf(stdout, NULL);

    // コマンドライン引数1つずつに子プロセスを作成
    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork");
        case 0: // 子プロセスはスリープするだけで終了
            printf("[%s] child %d started with PID %ld, sleeping %s "
                    "seconds\n", currTime("%T"), j, (long)getpid(),
                    argv[j]);
            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
            _exit(EXIT_SUCCESS);
        default:
            // 親プロセスは子プロセスを全て作成するまで繰り返す
            break;
        }
    }
    
    numDead = 0;

    // 親プロセスは全ての子プロセスの終了を待つ
    for (;;) {
        childPid = wait(NULL);
        if (childPid == -1) {
            if (errno == ECHILD) {
                // 子プロセスが存在しない場合
                printf("No more children - bye!\n");
                exit(EXIT_SUCCESS);
            } else {
                // wait()が失敗した場合
                errExit("wait");
            }
        }

        numDead++;
        printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
                currTime("%T"), (long)childPid, numDead);
    }
}
