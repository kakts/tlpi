/**
 * @file simple_thread.c
 * 29-1.ex Pthreadを用いたスレッドの作成 detach版
 */

#include <pthread.h>
#include "tlpi_hdr.h"

static void * threadFunc(void *arg)
{
    char *s = (char *) arg;
    printf("%s", s);

    int res;
    // 自スレッドをデタッチする
    res = pthread_detach(pthread_self());
    if (res != 0) {
        errExitEN(res, "pthread_detach");
    }
    
    return (void *) strlen(s);
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    void *res = NULL;

    int s; 

    // スレッドの作成
    // 作成されたスレッドは第三引数の関数に第4引数の値を渡して実行する
    s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");

    // スレッド作成失敗
    if (s != 0) {
        errExitEN(s, "pthread_create");
    }
    sleep(3);
    printf("Message from main()\n");

    // 作成したスレッドの終了を待つ
    // スレッドによって実行された関数の戻り値を&resに格納する
    s = pthread_join(t1, &res);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }

    printf("Thread returned %ld\n", (long) res);
    exit(EXIT_SUCCESS);
}