#include <time.h>
#include "curr_time.h"

#define BUF_SIZE 1000

// formatに指定された形式でフォーマットした現在時刻を返す。
// 変換仕様についてはstrftime(3)を参照
// formatがNULLならば %c を使用する
// エラー時はNULLを返す
char * currTime(const char *format)
{
  static char buf[BUF_SIZE];
  time_t t;
  size_t s;

  struct tm *tm;

  t = time(NULL);
  tm = localtime(&t);
  if (tm == NULL) {
    return NULL;
  }

  // format指定のない場合は%cを使う
  s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

  return (s == 0) ? NULL : buf;
}
