/**
 * @file t_mmap.c
 * 49-2 mmap()による共有ファイルマッピング
 * 
 * 先頭コマンドラインパラメータに与えられたファイルをマッピングし、マッピングの先頭にある値を表示
 * コマンドラインパラメータをもう1つ与えると、その文字列を共有マッピングへコピーする
 * 
 * 実行例
 * 1: まずサイズが1024バイトのファイルを作成
 * $ dd if=/dev/zero of=s.txt bs=1 count=1024
 * 2: t_mmapの実行
 * $ ./t_mmap s.txt hello
 */
#include <sys/mman.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define MEM_SIZE 10

int main(int argc, char *argv[])
{
    char *addr;
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s file [new-value]\n", argv[0]);
    }

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        errExit("open");
    }

    // 共有ファイルマッピング(MAP_SHARED)を作成
    addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        errExit("mmap");
    }

    // fdは不要になったのでクローズ
    if (close(fd) == -1) {
        errExit("close");
    }

    // セキュリティ対策 表示バイト数をMEM_SIZEに制限
    printf("Current string=%.*s\n", MEM_SIZE, addr);

    if (argc > 2) {
        // 2番目の引数で指定された文字列がMEM_SIZEバイトに収まるか確認
        if (strlen(argv[2]) >= MEM_SIZE) {
            cmdLineErr("new-value too large\n");
        }

        // 0で初期化
        memset(addr, 0, MEM_SIZE);
        strncpy(addr, argv[2], MEM_SIZE - 1);
        if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
            errExit("msync");
        }
        printf("Copied \"%s\" to shared memory\n", argv[2]);
    }

    exit(EXIT_SUCCESS);
}
