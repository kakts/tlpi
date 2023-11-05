/**
 * @file fork_file_sharing.c
 * 
 * 24-2 fork()後に親子プロセスがオープンしたファイル情報を共有する例
 * 
 * 作成済みのテンポラリファイルのファイルディスクリプ他に対して、
 * 子プロセスでオフセットを変更し、O_APPENDフラグを追加する
 * 親プロセスで、上記の子プロセスで行った変更が見えることを確認する
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    // stdoutのバッファリング停止
    setbuf(stdout, NULL);

    // テンポラリファィルを作成
    fd = mkstemp(template);
    if (fd == -1) {
        errExit("mkstemp");
    }

    printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

    // ファイルディスクリプタのステータスフラグを取得
    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        errExit("fcntl - F_GETFL");
    }

    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0: // 子プロセス オフセットとフラグを変更
        // テンポラリファイルに対するオフセットを変更
        if (lseek(fd, 1000, SEEK_SET) == -1) {
            errExit("lseek");
        }

        // 現在のフラグを取得
        flags = fcntl(fd, F_GETFL); // フラグを取得
        if (flags == -1) {
            errExit("fcntl - F_GETFL");
        }
        flags |= O_APPEND; // O_APPENDフラグを追加

        // ファイルディスクリプタに対して新たにステータスフラグを設定
        if (fcntl(fd, F_SETFL, flags) == -1) {
            errExit("fcntl - F_SETFL");
        }

        _exit(EXIT_SUCCESS);
    
    default: // 親プロセス: 子プロセスが加えた変更が見えることを確認
        // 子プロセスのwait
        if (wait(NULL) == -1) {
            errExit("wait");
        }

        printf("Child has exited\n");

        printf("File offset in parent: %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            errExit("fcntl - F_GETFL");
        }

        printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
        exit(EXIT_SUCCESS);
    }
}
