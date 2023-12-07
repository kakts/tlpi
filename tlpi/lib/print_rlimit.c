/**
 * @file print_rlimit.c
 * 36-2 リソース消費制限の表示
 */
#include <sys/resource.h>
#include "print_rlimit.h"
#include "tlpi_hdr.h"

/**
 * msgとresourceの値を表示
 */
int printRlimit(const char *msg, int resource)
{
    struct rlimit rlim;

    if (getrlimit(resource, &rlim) == -1) {
        return -1;
    }

    printf("%s soft=", msg);
    // 現在のソフトリミットが無制限の場合
    if (rlim.rlim_cur == RLIM_INFINITY) {
        printf("infinite");
// システム依存の処理
#ifdef RLIMI_SAVED_CUR
    } else if (rlim.rlim_cur == RLIM_SAVED_CUR) {
        printf("unrepresentable");
#endif
    } else {
        printf("%lld", (long long) rlim.rlim_cur);
    }

    printf("; hard=");
    // 現在のハードリミットが無制限の場合
    if (rlim.rlim_max == RLIM_INFINITY) {
        printf("infinite\n");
// システム依存
#ifdef RLIM_SAVED_MAX
    } else if (rlim.rlim_max == RLIM_SAVED_MAX) {
        printf("unrepresentable\n");    
#endif
    } else {
        printf("%lld\n", (long long) rlim.rlim_max);
    }
    return 0;
}