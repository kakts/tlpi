/**
 * @file test_become_daemon.c
 * 37-1, 37-2 become_daemon.cのテスト
 */
#include "become_daemon.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    becomeDaemon(0);
    
    sleep((argc > 1) ? getInt(argv[1], GN_NONNEG, "sleep-time") : 20);
    exit(EXIT_SUCCESS);
}
