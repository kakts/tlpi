/**
 * @file t_execle.c
 * 27-4 execle() リストによるパラメータ指定
 */

#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    char *envVec[] = { "GREET=salut", "BYE=adieu", NULL};
    char *filename;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s pathname\n", argv[0]);
    }

    // argv[1]からbasenameを取得
    filename = strrchr(argv[1], '/');
    if (filename != NULL) {
        filename++;
    } else {
        filename = argv[1];
    }

    execle(argv[1], filename, "hello world", (char *) NULL, envVec);
    errExit("execle");
}