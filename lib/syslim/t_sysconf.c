// 11.2 sysconf()を用いてシステムの様々な制限値を表示する

#include "../tlpi_hdr.h"

// sysconf(name)の戻り値をmsgとともに出力
static void sysconfPrint(const char *msg, int name)
{
  long lim;

  // sysconfがエラー時にはerrnoは0以外の値になるので、実行前に0に初期化
  errno = 0;
  lim = sysconf(name);
  if (lim != -1) {
    // 成功
    printf("%s %ld\n", msg, lim);
  } else {
    if (errno == 0) {
      // 成功したが値不明
      printf("%s (indeterminate)\n", msg);
    } else {
      errExit("sysconf %s", msg);
    }
  }
}

int main(int argc, char *argv[])
{
  sysconfPrint("_SC_ARG_MAX:     ", _SC_ARG_MAX);
  sysconfPrint("_SC_LOGIN_NAME_MAX:     ", _SC_LOGIN_NAME_MAX);
  sysconfPrint("_SC_OPEN_MAX:     ", _SC_OPEN_MAX);
  sysconfPrint("_SC_NGROUPS_MAX:     ", _SC_NGROUPS_MAX);
  sysconfPrint("_SC_PAGESIZE:     ", _SC_PAGESIZE);
  sysconfPrint("_SC_RTSIG_MAX:     ", _SC_RTSIG_MAX);
  exit(EXIT_SUCCESS);
}
