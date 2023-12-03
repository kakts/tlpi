/**
 * @file t_setpriority.c
 * 35-1 setpriority()の使用例
 * 引数の値を使用してプロセスの優先度を設定する
 * 
 * 引数に渡す値はnice値となる
 */
#include <sys/time.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int which, prio;
    id_t who;

    if (argc != 4 || strchr("pgu", argv[1][0]) == NULL) {
        usageErr("%s {p|g|u} who priority\n"
                 "    set priority of: p=process; g=process group; "
                 "u=processes for user\n", argv[0]);
    }

    // コマンドラインパラメータに応じnice値を設定する
    which = (argv[1][0] == 'p') ? PRIO_PROCESS :
            (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
    who = getLong(argv[2], 0, "who");
    prio = getInt(argv[3], 0, "prio");

    // nice値を設定する
    if (setpriority(which, who, prio) == -1) {
        errExit("setpriority");
    }

    // 変更内容を確認
    errno = 0;
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0) {
        errExit("getpriority");
    }
    printf("Nice value = %d\n", prio);
    exit(EXIT_SUCCESS);
}
