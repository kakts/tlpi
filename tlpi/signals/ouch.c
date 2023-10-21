/**
 * @file ouch.c
 * 20-1 Signal handler for SIGINT
 * 
 * Push Control-\ for quitting.
 */

#include <signal.h>
#include "../lib/tlpi_hdr.h"

/**
 * シグナルハンドラ関数
 */
static void sigHandler(int sig)
{
    printf("Ouch!\n");
}

int main(int argc, char *argv[])
{
    int i;

    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        errExit("signal");
    }

    for (i = 0; ; i++) {
        printf("%d\n", i);
        sleep(3);
    }
}