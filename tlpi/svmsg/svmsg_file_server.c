/**
 * @file svmsg_file_server.c
 * System V メッセージキューを利用したファイルサーバ
 */
#include "svmsg_file.h"

/**
 * SIGCHLDハンドラ
 */
static void grimReaper(int sig)
{
    int savedErrno;

    // waitpid()を実行するが、errnoを変更する恐れがあるため別に保存しておく
    savedErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }

    errno = savedErrno;    
}

/**
 * 子プロセスが実行する関数、クライアントごとに対応
 */
static void serveRequest(const struct requestMsg *req)
{
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE);
    }

    /**
     * メッセージ種類をRESP_MT_DATAとし、ファイル内容を送信。
     * クライアントへ通知する手段がないため、read() msgsnd()のエラーは特に対処しない
     */
    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0) {
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1) {
            break;
        }
    }

    resp.mtype = RESP_MT_END;
    msgsnd(req->clientId, &resp, 0, 0);
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    // サーバのメッセージキューを作成
    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWGRP);

    if (serverId == -1) {
        errExit("msgget");
    }

    // 終了した子プロセスを消滅させるSIGCHLDハンドラ設定
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        errExit("sigaction");
    }

    /**
     * リクエストを読み取り、個別の子プロセスで対応
     */
    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR) {
                continue;
            }

            errMsg("msgrcv");
            break;
        }

        pid = fork();
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }
    }

    // msgrcv() fork()がエラーになった場合はメッセージキューを削除し終了
    if (msgctl(serverId, IPC_RMID, NULL) == -1) {
        errExit("msgctl");
    }
    exit(EXIT_SUCCESS);
}

