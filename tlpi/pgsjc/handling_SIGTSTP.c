/**
 * @file handling_SIGTSTP.c
 * 34-6 SIGTSTPの捕捉
 * 
 * 下記手順により正しくSIGTSTPを処理するシグナルハンドラの例
 * 
 * 1: ハンドラがSIGTSTPのシグナル動作をデフォルト(SIG_DFL)へリセットする
 * 2: ハンドラがSIGTSTPを自プロセスへ送信する
 * 3: ハンドラ開始時点ですでにSIGTSTPはブロックされているため(SA_NODEFERフラグをセットしない限り)
 * 、ハンドラはSIGTSTPをアンブロックする。この時、前項で自プロセス宛に送信し、保留中のSIGTSTPのデフォルト
 * 動作が実行され、プロセスは一時停止する
 * 4: プロセスがSIGCONTを受信し、実行を再開するとハンドラ処理が継続される。
 * 5: ハンドラはSIGTSTPを再度ブロックし、以降のSIGTSTP処理に備え、自身をシグナル動作として再設定し、リターンする
 *
 * 実行方法
 * ./handling_SIGTSTP
 * Ctrl-Zを入力してSIGTSTPを生成する 
 *
 * その後、 fgにより SIGCONTを送信し、プロセス再開させる
 */
#include <signal.h>
#include "tlpi_hdr.h"

// SIGTSTPハンドラ
static void tstpHandler(int sig)
{
    sigset_t tstpMask, prevMask;
    int savedErrno;
    struct sigaction sa;

    // この関数の処理でerrnoを上書きしてしまう場合に備えて退避
    savedErrno = errno;

    printf("Caught SIGTSTP\n");

    // SIGTSTPのシグナル動作をデフォルト(SIG_DFL)へリセットする
    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR) {
        errExit("signal");
    }

    // 再度SIGTSTPを送信する
    raise(SIGTSTP);

    // SIGTSTPをアンブロックする
    // 保留中のSIGTSTPが自プロセスを一時停止させる
    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1) {
        errExit("sigprocmask");
    }

    // ここからはSIGCONTによる実行再開後の処理
    
    // SIGTSTPを再ブロック
    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1) {
        errExit("sigprocmask");
    }
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    // ハンドラを再設定
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    printf("Exiting SIGTSTP handler\n");
    errno = savedErrno;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    // SIGTSTPが無視されていない場合のみ、ハンドラを設定
    if (sigaction(SIGTSTP, NULL, &sa) == -1) {
        errExit("sigaction");
    }

    if (sa.sa_handler != SIG_IGN) {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = tstpHandler;
        if (sigaction(SIGTSTP, &sa, NULL) == -1) {
            errExit("sigaction");
        }
    }

    for (;;) {
        pause();
        printf("Main\n");
    }
}
