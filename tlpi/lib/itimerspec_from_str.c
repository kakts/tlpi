/**
 * @file itimerspec_from_str.c
 * 23-6
 */
#define POSIX_C_SOURCE 199309
#include <string.h>
#include <stdlib.h>
#include "itimerspec_from_str.h"


/**
 * 次の形式の文字列をitimerspecs tructに変換する
 * "value.sec[/value.nanosec][:interval.sec[/interval.nanosec]]"
 * 
 * 値を省略したフィールドは0とする
 */
void itimerspecFromStr(char *str, struct itimerspec *tsp)
{
    char *cptr, *sptr;

    cptr = strchr(str, ':');
    if (cptr != NULL) {
        *cptr = '\0';
    }

    sptr = strchr(str, '/');
    if (sptr != NULL) {
        *sptr = '\0';
    }

    // タイマーの初期値を設定
    tsp->it_value.tv_sec = atoi(str);
    tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

    if (cptr == NULL) {
        // インターバルは0
        tsp->it_interval.tv_sec = 0;
        tsp->it_interval.tv_nsec = 0;
    } else {
        // インターバルを設定
        sptr = strchr(cptr + 1, '/');
        if (sptr != NULL) {
            *sptr = '\0';
        }

        tsp->it_interval.tv_sec = atoi(cptr + 1);
        tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
    }
}