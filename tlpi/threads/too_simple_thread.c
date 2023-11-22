/**
 * @file too_simple_thread.c
 *
 */

#include <pthread.h>
#include <stdio.h>

static void * threadFunc(void *arg)
{
    char *s = (char *) arg;
    printf("%s", s);
    return (void *) strlen(s);
}

int main(int argc, char *argv[])
{
    pthread_t t1;

    int s; 

    // スレッドの作成
    // 作成されたスレッドは第三引数の関数に第4引数の値を渡して実行する
    s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");

    // スレッド作成失敗
    if (s != 0) {
        printf("pthread_create failed\n");
        return -1;
    }
    printf("pthread_create succeeded\n");
    return 0;
}