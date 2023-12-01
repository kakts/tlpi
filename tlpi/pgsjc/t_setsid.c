/**
 * @file t_setsid.c
 * 34-2 セッションの新規作成
 * setsid()を用いてセッションの新規作成を行う
 * 
 * /dev/ttyをオープンし、制御端末を持たなくなったことを確認する
 */

#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    // 親プロセスまたエラーの場合は終了
    if (fork() != 0) {
        exit(EXIT_SUCCESS);
    }

    // setsid()失敗の場合
    if (setsid() == -1) {
        errExit("setsid");
    }

    printf("PID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(), (long) getpgrp(), (long) getsid(0));

    // /dev/ttyをオープンし、制御端末を持たなくなったことを確認する
    if (open("/dev/tty", O_RDWR) == -1) {
        errExit("open /dev/tty");
    }
    exit(EXIT_SUCCESS);
}
