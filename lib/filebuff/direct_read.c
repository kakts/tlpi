// O_DIRECTを用いてファイルを読み取るサンプル

#define _GNU_SOURCE  // <fcntl.h>内のO_DIRECT定義を有効にする
#include <fcntl.h>
#include <malloc.h>
#include "../tlpi_hdr.h"

int main(int argc, char *argv[])
{
  int fd;
  ssize_t numRead;
  size_t length, alignment;
  off_t offset;
  char *buf;

  if (argc < 3 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s file length [offset [alignment]]\n", argv[0]);
  }

  length = getLong(argv[2], GN_ANY_BASE, "length");
  offset = (argc > 3) ? getLong(argv[3], GN_ANY_BASE, "offset") : 0;
  alignment = (argc > 4) ? getLong(argv[4], GN_ANY_BASE, "alignment") : 4096;

  // O_DIRECT フラグを付けてオープンする
  fd = open(argv[1], O_RDONLY | O_DIRECT);
  if (fd == -1) {
    errExit("open");
  }

  // memalign()は先頭引数の整数倍となるようアライメントされたメモリ領域を割り当てる
  // 先頭引数を alignment * 2とし、戻り値のポインタにalignmentを加えることで
  // buf は alignmentのべき乗倍とならない
  // 例えば、256バイトでアラインメントする場合に 512バイトでアラインメントするのを防ぐためである
  buf = memalign(alignment * 2, length + alignment);
  if (buf == NULL) {
    errExit("memalign");
  }

  buf += alignment;

  if (lseek(fd, offset, SEEK_SET) == -1) {
    errExit("lseek");
  }

  numRead = read(fd, buf, length);
  if (numRead == -1) {
    errExit("read");
  }
  printf("Read %ld bytes\n", (long) numRead);

  exit(EXIT_SUCCESS);
}
