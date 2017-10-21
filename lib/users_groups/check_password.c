#define _BSD_SOURCE // <unistd.h>内のgetpass()の宣言を有効にする

#define _XOPEN_SOURCE // <unistd.h>内のcrypt()の宣言を有効にする

#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "../tlpi_hdr.h"

int main(int argc, char *argv[])
{
  char *username, *password, *encrypted, *p;

  struct passwd *pwd;
  struct spwd *spwd;
  Boolean authOk;
  size_t len;
  long lnmax;

  lnmax = sysconf(_SC_LOGIN_NAME_MAX);
  if (lnmax == -1) {
    // 最小ログイン名長が不明な場合 推測
    lnmax = 256;
  }

  username = malloc(lnmax);
  if (username == NULL) {
    errExit("malloc");
  }

  printf("Username: ");
  fflush(stdout);
  if (fgets(username, lnmax, stdin) == NULL) {
    // EOFならば終了
    exit(EXIT_FAILURE);
  }

  len = strlen(username);
  if (username[len - 1] == '\n') {
    // 行末を削除
    username[len - 1] = '\0';
  }

  pwd = getpwnam(username);
  if (pwd == NULL) {
    fatal("Couldn't get password record");
  }

  spwd = getspnam(username);
  if (spwd == NULL && errno == EACCES) {
    fatal("No permission to read shadow password file");
  }

  if (spwd != NULL) {
    // shadowパスワードが存在すれば shadowパスワードを利用する
    pwd->pw_passwd = spwd->sp_pwdp;
  }
  password = getpass("Password: ");

  // パスワードを暗号化し、平文は即座に破棄する
  encrypted = crypt(password, pwd->pw_passwd);
  for (p = password; *p != '\0';) {
    *p++ = '\0';
  }

  if (encrypted == NULL) {
    errExit("crypt");
  }

  // 入力されたパスワードを暗号化したものと、shadowパスワードが一致するかチェック
  authOk = strcmp(encrypted, pwd->pw_passwd) == 0;

  if (!authOk) {
    printf("Incorrect password\n");
    exit(EXIT_FAILURE);
  }

  printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

  // 認証成功 ここで処理を行う
  printf("Now do authenticated work");

  exit(EXIT_SUCCESS);
}
