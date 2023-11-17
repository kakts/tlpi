/**
 * @file closeonexec.c
 * 27-6 ファイルディスクリプタへのclose-on-execフラグセット
 * 
 * 引数が1つ以上ある場合、 stdoutに対してclose-on-execフラグをセットし、
 * execlpによりlsを実行する
 * フラグがセットされた場合、lsの実行後にstdoutはクローズされため、lsでエラーが出る
 * 
 * stdoutをクローズせずlsを実行する
 * ./closeonexec
 * 
 * stdoutへclose-on-execフラグをセットする
 * ./closeonexec n
 */

#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int flags;

    if (argc > 1) {
        // フラグの取得
        flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1) {
            errExit("fcntl - F_GETFD");
        }

        // FD_CLOEXECフラグをセットし直すため、フラグ追加
        flags |= FD_CLOEXEC;

        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) {
            errExit("fcntl - F_SETFD");
        }
    }

    execlp("ls", "ls", "-l", argv[0], (char *) NULL);
    errExit("execlp");
}
