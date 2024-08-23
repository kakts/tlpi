/**
 * @file us_xfr.h
 */
#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

/**
 * UNIXドメインソケットのパス
 */
#define SV_SOCK_PATH "/tmp/us_xfr"

#define BUF_SIZE 100