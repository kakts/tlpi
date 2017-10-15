#include <unistd.h>
#include <fcntl.h>
#include "../tlpi_hdr.h"

// Chapter5.8
// truncateのサンプルコード
// truncateは、open()またはそれ以外の方法で得たファイルディスクリプタを用いずに
// ファイル内容を変更できるという珍しい特徴を持った唯一のシステムコール
int main(int argc, char *argv[])
{
  int result_truncate, size_length;

  // 変更後のファイルサイズを指定する
  size_length = atoi(argv[2]);

  printf("size_length: %d\n", size_length);

  result_truncate = truncate(argv[1], size_length);
  if (result_truncate == -1) {
    printf("truncate file failed\n");
    errExit("truncate");
  }

  printf("success\n");
  exit(EXIT_SUCCESS);
}
