/**
 * @file is_echo_sv.c
 * 60-4 TCP echo用の並行サーバ
 */
#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include "../lib/become_daemon.h"
#include "inet_sockets.h"
#include "tlpi_hdr.h"

#define SERVICE "echo"  // サービス名
#define BUF_SIZE 4096

/**
 * 終了した子プロセスを消滅させるSIGCHLDハンドラ
 */
static void grimReaper(int sig)
{

    // 変更される可能性があるので、errnoを保存
    int savedErrno;

    savedErrno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = savedErrno;
}

/**
 * クライアントからの接続を処理する
 * ソケットからの入力を同じソケットへ送り返す
 */
static void handleRequest(int cfd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
        if (write(cfd, buf, numRead) != numRead) {
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    // リスニングソケットと接続済みソケット
    int lfd, cfd;

    struct sigaction sa;

    if (becomeDaemon(0) == -1) {
        errExit("becomeDaemon");
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        syslog(LOG_ERR, "Error from sigaction(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    lfd = inetListen(SERVICE, 10, NULL);
    if (lfd == -1) {
        syslog(LOG_ERR, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;) {
        // 接続要求を待つ
        cfd = accept(lfd, NULL, NULL);
        if (cfd == -1) {
            syslog(LOG_ERR, "Error from accept(): %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        // クライアントは専用の新規子プロセスで処理
        switch (fork()) {
            case -1:
                syslog(LOG_ERR, "Error from fork(): %s", strerror(errno));
                close(cfd);
                // 一時的なエラーの可能性もある。次のクライアントを処理
                break;
            case 0:
                // 子プロセス
                close(lfd);
                handleRequest(cfd);
                _exit(EXIT_SUCCESS);
            default:
                // 親プロセス
                close(cfd);
                break;
        }
    }
}
