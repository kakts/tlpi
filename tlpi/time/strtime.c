/**
 * 10.2.3
 * 日付と時刻を含む文字列をコマンドラインパラメータとして受取
 * strptime()によりtm構造体へ変換
 * さらにstrftime()を用いて逆変換した値を返す
 */

#define _XOPEN_SOURCE
#include <time.h>
#include <locale.h>
#include "../lib/tlpi_hdr.h"

#define SBUF_SIZE 1000

int main(int argc, char *argv[])
{
  struct tm tm;
  char sbuf[SBUF_SIZE];
  char *ofmt;

  if (argc < 3 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s input-date-time in-format [out-format]\n", argv[0]);
  }

  // 変換時にlocaleを参照する
  if (setlocale(LC_ALL, "") == NULL) {
    errExit("setlocale");
  }

  // tmの初期化
  memset(&tm, 0, sizeof(struct tm));

  // 文字列を指定したフォーマットに従い解析する
  // &tmに変換後のデータを返す
  // argv[1] 日付の文字列 argv[2] フォーマット
  if (strptime(argv[1], argv[2], &tm) == NULL) {
    fatal("strptime");
  }

  // strptime()ではtm_isdstが代入されない。 mktime()にDSTを判断させる
  tm.tm_isdst = -1;

  printf("calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));

  // formatが指定されていない場合はデフォルト
  ofmt = (argc > 3) ? argv[3] : "%H:%M:%s %A, %d %B %Y %Z";

  // tmから新たなフォーマットに再変換し文字列にする
  if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0) {
    fatal("strftime returned 0");
  }

  // 書き出し成功した内容を表示
  printf("strftime() yields: %s \n", sbuf);
  exit(EXIT_SUCCESS);
}
