/**
 * @file svmsg_create.c
 * 46-1 System Vメッセージキュー msgget()の使用例
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

/**
 * 使用法を表示し終了
 */
static void usegeError(const char *progName, const char *msg)
{
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }
    fprintf(stderr, "Usage: %s [-cx] {-f pathname | -k key | -p} [octal-perms]\n", progName);
    fprintf(stderr, "    -c           Use IPC_CREAT flag\n");
    fprintf(stderr, "    -x           Use IPC_EXCL flag\n");
    fprintf(stderr, "    -f pathname  Generate key using ftok()\n");
    fprintf(stderr, "    -k key       Use 'key' as key\n");
    fprintf(stderr, "    -p           Use IPC_PRIVATE key\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    // オプションのカウント
    int numKeyFlags;
    int flags, msqid, opt;
    unsigned int perms;
    long lkey;
    key_t key;

    /**
     * コマンドラインオプション、パラメータの解析
     */

    numKeyFlags = 0;
    flags = 0;

    while ((opt = getopt(argc, argv, "cf:k:px")) != -1) {
        switch (opt) {
        case 'c':
            flags |= IPC_CREAT;
            break;
        case 'f':
            key = ftok(optarg, 1);
            if (key == -1) {
                errExit("ftok");
            }
            numKeyFlags++;
            break;
        case 'k':
            if (sscanf(optarg, "%li", &lkey) != 1) {
                cmdLineErr("-k option requires a numeric argument\n");
            }
            key = lkey;
            numKeyFlags++;
            break;
        case 'p':
            key = IPC_PRIVATE;
            numKeyFlags++;
            break;
        case 'x':
            flags |= IPC_EXCL;
            break;
        default:
            usegeError(argv[0], NULL);
        }
    }

    if (numKeyFlags != 1) {
        usegeError(argv[0], "Exactly one of the options -f, -k, or -p must be supplied\n");
    }

    perms = (optind == argc) ? (S_IRUSR | S_IWUSR) : getInt(argv[optind], GN_BASE_8, "octal-perms");

    // System Vメッセージキューの作成
    msqid = msgget(key, flags | perms);
    if (msqid == -1) {
        errExit("msgget");
    }
    printf("%d\n", msqid);
    exit(EXIT_SUCCESS);
}
