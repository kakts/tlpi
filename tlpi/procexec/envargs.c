/**
 * @file evnargs.c
 * 
 * 27-2 コマンドラインパラメータと環境を表示する
 * procexec/t_execve.c からexecve()で呼び出す用
 */
#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char const *argv[])
{
    printf("envargs called. \n");
    int j;
    char **ep;

    for (j = 0; j < argc; j++) {
        printf("argv[%d] = %s\n", j, argv[j]);
    }

    for (ep = environ; *ep != NULL; ep++) {
        printf("environ: %s\n", *ep);
    }
    exit(EXIT_SUCCESS);
}
