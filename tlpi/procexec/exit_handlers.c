/**
 * @file exit_handlers.c
 * 25-1 exit handlers
 * 
 * exit ハンドラの確認
 * on_exit、atexit()に登録すると、exit()を実行した際に最後に登録した順にハンドラーが実行される
 */

#define _BSD_SOURCE
#include <stdlib.h>
#include "tlpi_hdr.h"

static void atexitFunc1(void)
{
    printf("atexit function 1 called\n");
}

static void atexitFunc2(void)
{
    printf("atexit function 2 called\n");
}

static void onexitFunc(int exitStatus, void *arg)
{
    printf("on_exit function called: status=%d, arg=%ld\n", exitStatus, (long)arg);
}

int main(int argc, char const *argv[])
{
    if (on_exit(onexitFunc, (void *)10) != 0)
        fatal("on_exit 1");
    if (atexit(atexitFunc1) != 0)
        fatal("atexit 1");
    if (atexit(atexitFunc2) != 0)
        fatal("atexit 2");
    if (on_exit(onexitFunc, (void *)20) != 0)
        fatal("on_exit 2");
    exit(2);
}

