/**
 * @file simple_system.c
 * 
 * 27-8 シグナルを処理しないsysmte()の実装
 */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int system(char *command)
{
    int status;
    pid_t childPid;

    switch (childPid = fork()) {
    case -1:
        return -1;
    case 0:
        // 子プロセス
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _exit(127);
    default: // 親プロセス
        if (waitpid(childPid, &status, 0) == -1) {
            return -1;
        }
        return status;
        
    }
}