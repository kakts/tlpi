/**
 * @file read_line.c
 * 59-1 行単位のデータ読み取り
 */
#include <unistd.h>
#include <errno.h>
#include "read_line.h"

/**
 * fdに指定されたファイルディスクリプタから、改行文字までを読み取り、
 * bufferに指定されたバッファへ返す
 * bufferにはnバイト以上のバッファを指定する
 * bufferに返される文字列は常にNULLで終端される。つまり文字列の最大長は(n - 1)
 * 戻り値は、終端のNULLを含まない、bufferに返したバイト数
 */
ssize_t readLine(int fd, void *buffer, size_t n)
{
    
    ssize_t numRead; // read()で読み取ったバイト数
    size_t totRead; // 読み取ったバイト総数

    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    // void *には算術演算しない
    buf = buffer;
    totRead = 0;

    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            // 割り込まれた場合再度ループして読み取り
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (numRead == 0) {
            // EOFに達した場合

            // 1バイトも読み取れない場合0を返す
            if (totRead == 0) {
                return 0;
            }
            break;
        } else {
            // ここに到達した場合numReadは必ず1
            
            // (n - 1) バイト目以降は破棄
            if (totRead < n - 1) {
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n') {
                break;
            }
        }
    }

    *buf = '\0'; // 文字列の終端を設定
    return totRead;
}