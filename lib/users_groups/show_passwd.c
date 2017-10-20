#include <pwd.h>
#include "../tlpi_hdr.h"

void main(int argc, char *argv[])
{
  struct passwd *pwd;

  // getpwent()の戻り値はパスワードファイル内の情報を1つずつ返す
  // パスワードの終端に達した場合NULLを返す
  while ((pwd = getpwent()) != NULL) {
    printf("%-8s %5ld %s\n ", pwd->pw_name, (long) pwd->pw_uid, pwd->pw_dir);
  }

  // endpwent()によりパスワードファイルがクローズされる
  endpwent();
}
