/**
 * @file strerror.c
 * 31-1 非スレッドセーフなstrerror()の実装
 * 
 * glibcが定義する2つのグローバル変数を使用する
 * 
 * _sys_nerr, _sys_errlistはdeprecatedのため 利用不可
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_ERROR_LEN 256

// 戻り値となるスタティックバッファ
static char buf[MAX_ERROR_LEN];

char * strerror(int err)
{
    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';
    }
    return buf;
}
