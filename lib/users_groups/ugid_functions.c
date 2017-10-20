// user group関連のシステムコールのサンプル

#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "ugid_functions.h"
#include "../tlpi_hdr.h"

// uidに対応する名前を取得する エラー時にはNULLを返す
char *userNameFromId(uid_t uid)
{
  struct passwd *pwd;

  pwd = getpwuid(uid);
  return (pwd == NULL) ? NULL : pwd->pw_name;
}

// 名前に対応するUIDを取得する エラー時には-1を返す
uid_t userIdFromName(const char *name)
{
  struct passwd *pwd;
  uid_t u;
  char *endptr;

  // nameがNULLか空文字ならエラー
  if (name == NULL || *name == '\0') {
    return -1;
  }

  // 利便性向上の為 数字文字列も受け付ける
  u = strtol(name, &endptr, 10);
  if (*endptr == '\0') {
    return u;
  }

  pwd = getpwnam(name);
  if (pwd == NULL) {
    return -1;
  }

  return pwd->pw_uid;
}

// gidに対応する名前を取得する エラー時にはNULL
char * groupNameFromId(gid_t gid)
{
  struct group *grp;

  // group情報取得
  grp = getgrgid(gid);
  return (grp == NULL) ? NULL : grp->gr_name;
}

// 名前に対応するGIDを、エラー時には-1を返す
gid_t groupIdFromName(const char *name)
{
  struct group *grp;
  gid_t g;
  char *endptr;

  // nameがNULLか空文字ならエラー
  if (name == NULL || *name == '\0') {
    return -1;
  }

  // 利便性向上の為 数字文字列も受け付ける
  g = strtol(name, &endptr, 10);
  if (*endptr == '\0') {
    return g;
  }

  grp = getgrnam(name);
  if (grp == NULL) {
    return -1;
  }

  return grp->gr_gid;
}
