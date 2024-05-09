/**
 * @file simple_svmsg_create.c
 * 46-1 System Vメッセージキュー msgget()の使用例 簡易版
 * 
 * 一意のIDで作成
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int msqid;

    // System Vメッセージキューの作成 既存の場合はそのメッセージキューIDを返す
    msqid = msgget(IPC_PRIVATE, IPC_CREAT);
    if (msqid == -1) {
        errExit("msgget");
    }
    printf("%d\n", msqid);
    exit(EXIT_SUCCESS);
}
