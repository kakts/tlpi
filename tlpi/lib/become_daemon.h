/**
 * @file become_daemon.h
 * 37-1 become_daemopn.cのヘッダ
 */

#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H

/**
 * becomeDaemon()のフラグ
 */
#define BD_NO_CHDIR           01
#define BD_NO_CLOSE_FILES     02
#define BD_NO_REOPEN_STD_FDS  04

#define BD_NO_UMASK0        010
#define BD_MAX_CLOSE        8192

int becomeDaemon(int flags);

#endif