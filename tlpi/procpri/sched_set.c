/**
 * @file sched_set.c
 * 35-2 スケジューリングポリシーと優先度の設定
 * 
 * sched_setscheduler()の使用例
 * 
 * コマンドライン引数で与えられたプロセスの優先度、スケジューリングポリシーを設定する
 * 
 * 対象のプロセスは複数指定可能
 */
#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    if (argc < 3 || strchr("rfo", argv[1][0]) == NULL) {
        usageErr("%s policy priority [pid...]\n"
                 "    policy is 'r' (RR), 'f' (FIFO), "
#ifdef SCHED_BATCH
                    "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE
                    "'i' (IDLE), "
#endif
                 "or 'o' (OTHER)\n", argv[0]);
    }

    pol = (argv[1][0] == 'r') ? SCHED_RR :
          (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH
          (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE
          (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
          SCHED_OTHER;
    sp.sched_priority = getInt(argv[2], 0, "priority");
    
    // 指定されたプロセスの数だけ、スケジューリングポリシーと優先度を設定する
    for (j = 3; j < argc; j++) {
        if (sched_setscheduler(getLong(argv[j], 0, "pid"), pol, &sp) == -1) {
            errExit("sched_setscheduler");
        }
    }

    exit(EXIT_SUCCESS);
}
