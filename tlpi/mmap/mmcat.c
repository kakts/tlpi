/**
 * @file mmcat.c
 * 49-1 mmap()による非共有ファイルマッピング
 * マッピング内のデータを変更しても、他プロセスには影響しない
 * 対応するファイルへも反映されない
 * 
 * 複数プロセス間で同じデータをマッピングした場合、他のプロセスで更新されるまでは同じだが
 * 更新する際にカーネルがそのプロセス用にページの複製を新規作成する(コピーオンライト)
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *addr;
    int fd;
    struct stat sb;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s file\n", argv[0]);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        errExit("open");
    }

    /**
     * ファイルサイズをマッピングサイズおよび出力バイト数とする
     */
    if (fstat(fd, &sb) == -1) {
        errExit("fstat");
    }

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        errExit("mmap");
    }

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size) {
        fatal("partial/failed write");
    }
    exit(EXIT_SUCCESS);
}
