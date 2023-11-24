/**
 * @file thread_multijoin.c
 * 
 * 30-4 任意のスレッドのjoin()
 * 
 * 複数のスレッドを生成し、任意のスレッドの終了を待つ
 */

#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

// 作成されたスレッドの総数
static int totThreads = 0;
// まだ生存中もしくは、停止したがjoin()まちのスレッド数
static int numLive = 0;

// 停止したがjoin()待ちのスレッド数
static int numUnjoined = 0;

// スレッドの状態を表す
enum tstate {
    TS_ALIVE, // スレッドはまだ生存中
    TS_TERMINATED, // スレッドは終了したが、まだjoin()されていない
    TS_JOINED // スレッドはjoin()された
};

// 各スレッドの状態
static struct {
    pthread_t tid; // スレッドID
    enum tstate state; // スレッドの状態
    int sleepTime; // 停止前に生存していた時間
} *thread;

static void * threadFunc(void *arg)
{
    int idx = (int) arg;
    int s;

    // 疑似処理
    sleep(thread[idx].sleepTime);
    printf("Thread %d terminating\n", idx);

    s = pthread_mutex_lock(&threadMutex);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_lock");
    }

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0) {
        errExitEN(s, "pthread_mutex_unlock");
    }
    
    // スレッドの終了を待っているスレッドに通知する
    s = pthread_cond_signal(&threadDied);
    if (s != 0) {
        errExitEN(s, "pthread_cond_signal");
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    int s, idx;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s nsecs...\n", argv[0]);
    }

    // 引数で指定した数のスレッド用のメモリを確保する
    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL) {
        errExit("calloc");
    }

    // スレッドを作成する
    for (idx = 0; idx < argc - 1; idx++) {
        thread[idx].sleepTime = getInt(argv[idx + 1], GN_NONNEG, NULL);
        thread[idx].state = TS_ALIVE;
        s = pthread_create(&thread[idx].tid, NULL, threadFunc, (void *) idx);

        if (s != 0) {
            errExitEN(s, "pthread_create");
        }
    }

    totThreads = argc - 1;
    numLive = totThreads;

    // 終了したスレッドをjoin()する
    while (numLive > 0) {
        // 条件変数を使用するためにmutexをロックする
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_lock");
        }

        while (numUnjoined == 0) {
            // 条件変数を待機し、スレッドが終了した通知を待つ
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if (s != 0) {
                errExitEN(s, "pthread_cond_wait");
            }
        }

        for (idx = 0; idx < totThreads; idx++) {
            // join()待ちのスレッドを探す
            if (thread[idx].state == TS_TERMINATED) {
                s = pthread_join(thread[idx].tid, NULL);
                if (s != 0) {
                    errExitEN(s, "pthread_join");
                }

                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        // mutexをアンロックする
        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0) {
            errExitEN(s, "pthread_mutex_unlock");
        }
    }

    exit(EXIT_SUCCESS);
}
