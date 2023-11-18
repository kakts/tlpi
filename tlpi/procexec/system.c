/**
 * @file system.c
 * 27-9 system()の実装
 */

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int system(const char *command)
{
    sigset_t blockMask, origMask;
    struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    pid_t childPid;
    int status, savedErrno;

    // 空コマンド、シェルが無効でないことを確認
    if (command == NULL) {
        return system(":") == 0;
    }

    // SIGCHLDをブロック
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blockMask, &origMask);

    // SIGINTとSIGQUITを無視するように設定
    saIgnore.sa_handler = SIG_IGN;
    saIgnore.sa_flags = 0;
    sigemptyset(&saIgnore.sa_mask);

    sigaction(SIGINT, &saIgnore, &saOrigInt);
    sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

    switch (childPid = fork()) {
    case -1:
        // エラー
        // シグナル動作を復元するためここでは終了しなし
        status = -1;
        break;
    case 0:
        saDefault.sa_handler = SIG_DFL;
        saDefault.sa_flags = 0;
        sigemptyset(&saDefault.sa_mask);

        if (saOrigInt.sa_handler != SIG_IGN) {
            sigaction(SIGINT, &saDefault, NULL);
        }
        if (saOrigQuit.sa_handler != SIG_IGN) {
            sigaction(SIGQUIT, &saDefault, NULL);
        }

        sigprocmask(SIG_SETMASK, &origMask, NULL);

        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _exit(127);
    
    default: // 親プロセス 子プロセスの終了を待つ
        while (waitpid(childPid, &status, 0) == -1) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
        break;
    }

    // SIGCHLDをアンブロックし、SIGINTとSIGQUITの動作を復元
    savedErrno = errno;

    sigprocmask(SIG_SETMASK, &origMask, NULL);
    sigaction(SIGINT, &saOrigInt, NULL);
    sigaction(SIGQUIT, &saOrigQuit, NULL);

    errno = savedErrno;

    return status;
}