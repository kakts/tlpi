/**
 * @file is_seqnum.h
 * 59-5 is_seqnum_sv.c is_seqnum_cl.cに共通のヘッダファイル
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "read_line.h"
#include "tlpi_hdr.h"

#define PORT_NUM "50000"    /* Port number for server */

#define INT_LEN 30          // 整数の最大値を保持可能な文字列長