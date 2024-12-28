/**
 * @file rdwrn.c
 * 61-1 readn() and writen() functions
 */
#include <unistd.h>
#include <errno.h>
#include "rdwrn.h"

/**
 * 指定したバイト数のデータを読み込む
 */
ssize_t readlink(int fd, void *buffer, size_t n)
{
    // 最後のread()が読み取ったバイト数
    ssize_t numRead;

    // 読み取ったバイト総数
    size_t totRead;

    char *buf;

    // void *の算術演算を避けるために、char *にキャスト 
    buf = buffer;

    for (totRead = 0; totRead < n;) {
        numRead = read(fd, buf, n - totRead);

        // EOF
        if (numRead == 0) {
            // 最初のread()ならば、0を返す
            reaturn totRead;
        }

        if (numRead == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        }
        totRead += numRead;
        buf += numRead;
    }
    return totRead;
}

/**
 * 指定したバイト数のデータを書き込む
 */
ssize_t writen(int fd, const void *buffer, size_t n)
{
    // 最後のwrite()が書き込んだバイト数
    ssize_t numWritten;
    // 書き込んだバイト総数
    size_t totWritten;
    const char *buf;

    // void *の算術演算を避けるために、char *にキャスト
    buf = buffer;

    for (totWritten = 0; totWritten < n;) {
        numWritten = write(fd, buf, n - totWritten);

        if (numWritten <= 0) {
            if (numWritten == -1 && errno == EINTR) {
                // 割り込まれた場合、再度書き込みを行う
                continue;
            } else {
                return -1;
            }
        }
        totWritten += numWritten;
        buf += numWritten;
    }
    return totWritten;
}
