/**
 * @file fork_stdio_buf.c
 * 25-2 fork()実行によるstdioバッファへの影響
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("Hello world\n");
    write(STDOUT_FILENO, "Ciao\n", 5);

    if (fork() == -1)
        errExit("fork");

    // 親子プロセスともにここまで実行する
    exit(EXIT_SUCCESS);
}
