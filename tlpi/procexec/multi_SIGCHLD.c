/**
 * @file multi_SIGCHLD.c
 * 26-5 SIGCHLDシグナルのハンドラで終了した子プロセスを消滅させる
 * 
 * 3つの子プロセスを作成する
 * ex: ./multi_SIGCHLD 1 2 4 
 */

#include <signal.h>
#include <sys/wait.h>
#include "print_wait_status.h"          /* Declares printWaitStatus() */
#include "tlpi_hdr.h"
#include "curr_time.h"

// 作成したが消滅させていない子プロセス数
static volatile int numLiveChildren = 0;

static void sigchldHandler(int sig)
{
    int status, savedErrno;
    pid_t childPid;

    // このハンドラは非同期シグナルセーフでない関数を使用するため、errnoが変更される場合に備えて退避させる
    savedErrno = errno;

    printf("%s handler: Caught SIGCHLD\n", currTime("%T"));
    
    // 子プロセスのID、状態を表示
    while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("%s handler: Reaped child %ld - ", currTime("%T"), (long) childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
    }


    if (childPid == -1 && errno != ECHILD) {
        errMsg("waitpid");
    }

    // 意図的に所要時間を延長する
    sleep(5);

    printf("%s handler: returning\n", currTime("%T"));
    errno = savedErrno;

}

int main(int argc, char const *argv[])
{
    int j, sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;

    // 引数チェック
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s child-sleep-time...\n", argv[0]);
    }

    setbuf(stdout, NULL);

    sigCnt = 0;
    numLiveChildren = argc - 1;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    // 親プロセスがsigsuspend()前に子プロセスが終了する可能性があるため、SIGCHLDをブロックする

    // blockMaskを空にする
    sigemptyset(&blockMask);
    // blockMaskにSIGCHLDを追加する
    sigaddset(&blockMask, SIGCHLD);

    // プロセスのシグナルマスクをblockMaskに設定する
    // SIG_SETMASKは新たに指定したマスクで現在のマスクを置き換える
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1) {
        errExit("sigprocmask");
    }


    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork");
        case 0: // 子プロセス スリープして終了するだけ
            sleep(getInt(argv[j], GN_NONNEG, "child-sleep-time"));
            printf("%s Child %d (PID=%ld) exiting\n", currTime("%T"), j, (long) getpid());
            _exit(EXIT_SUCCESS);
        default: // 親プロセス
            break;
        }
    }

    // ここに到達するのは親プロセスのみ 全ての子プロセスが終了するまでSIGCHLDを待つ
    sigemptyset(&emptyMask);
    while (numLiveChildren > 0) {
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
            errExit("sigsuspend");
        }
        sigCnt++;
    }

    printf("%s All %d children have terminated; SIGCHLD was caught %d times\n", currTime("%T"), argc - 1, sigCnt);
    exit(EXIT_SUCCESS);
}
