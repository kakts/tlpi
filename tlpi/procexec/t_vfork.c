/**
 * @file t_vfork.c
 * 
 * 24-4 vfork()の例
 */

#include "tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    int istack = 222;

    switch (vfork()) {
    case -1:
        errExit("vfork");
    case 0: // おやプロセスのメモリ空間を参照したままで、子プロセスが先に実行される
        sleep(3);

        write(STDOUT_FILENO, "Child executing\n", 16);
        
        // この変更は親プロセスからも見える
        istack *= 3;
        _exit(EXIT_SUCCESS);
    default:
        // 親プロセスは子プロセスが終了するまで実行されない
        write(STDOUT_FILENO, "Parent executing\n", 17);
        printf("istack=%d\n", istack);
        exit(EXIT_SUCCESS);
    }

}
