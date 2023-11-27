/**
 * @file become_daemon.c
 * 37-2 プロセスのデーモン化関数
 */

#include <sys/stat.h>
#include <fcntl.h>
#include "become_daemon.h"
#include "tlpi_hdr.h"

/**
 * デーモン化を行う
 */
int becomeDaemon(int flags)
{
    int maxfd, fd;

    // 子プロセスは実行継続するが、親プロセスは終了させる
    switch (fork()) {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    // ここからは子プロセスが実行

    // 新規セッションリーダー化
    if (setsid() == -1) {
        return -1;
    }

    // さらにフォークして、fork()を実行した子プロセスは終了し、孫プロセスは実行継続する
    switch (fork()) {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0)) {
        // ファイル作成マスクをクリアする
        umask(0);
    }

    if (!(flags & BD_NO_CHDIR)) {
        // カレントディレクトリをルートディレクトリに変更する
        chdir("/");
    }

    if (!(flags & BD_NO_CLOSE_FILES)) {
        // ファイルディスクリプタをクローズする
        // クローズするファイルディスクリプタの最大値を取得する
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1) {
            maxfd = BD_MAX_CLOSE;
        }

        // ファイルディスクリプタをクローズする
        for (fd = 0; fd < maxfd; fd++) {
            close(fd);
        }
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS)) {
        // 標準入出力、標準エラー出力を/dev/nullにリダイレクトする
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);
        if (fd != STDIN_FILENO) {
            return -1;
        }
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
            return -1;
        }
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
            return -1;
        }
    }

    return 0;
}