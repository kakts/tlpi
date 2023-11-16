/**
 * @file t_execl.c
 * 27-5 execl()した新規プログラムへ環境を渡す
 * execl()は、実行したプロセスの環境をそのまま引き継ぐ
 */

#include <stdlib.h>
#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    
    printf("Initial value of USER: %s\n", getenv("USER"));

    if (putenv("USER=britta") != 0) {
        errExit("putenv");
    }

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *) NULL);
    errExit("execl");
}

