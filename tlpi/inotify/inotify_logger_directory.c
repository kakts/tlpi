/**
 * inotifyを使ったファイル変更検知ロガー
 * 
 * inotifyによる監視対象
 * 特定のファイルを指定
 */

#include <sys/inotify.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include "../lib/tlpi_hdr.h"

#define WATCH_TARGET_DIR "./target"
#define LOG_FILE "./log.txt"

/**
 * 指定したファイルディスクリプタが指すファイルにinotify_event構造体の内容を書き込む
 */
static void writeToLog(int fd, struct inotify_event *p)
{
    printf("writeToLog\n");
    printf("p->len:%d\n", p->len);
    printf("p->name:%s\n", p->name);
    ssize_t numWrite;

    // write()を使ってp->nameをファイルに書き込む
    numWrite = write(fd, p->name, p->len);
    if (numWrite == -1) {
        errExit("write");
    }

    // 改行文字を書き込む
    numWrite = write(fd, "\n", 1);
    if (numWrite == -1) {
        errExit("write");
    }

    printf("Write %ld bytes to log file. name:%s\n", (long) numWrite, p->name);
}

/**
 * read()に指定したバッファサイズ小さく、次のinotify_event構造体を読み込めない場合がある
 * これを回避するために、inotify_eventを最低でも1つは保持できるだけのサイズ(sizeof(struct inotify_event) + NAME_MAX + 1)を確保すれば良い
 */
#define BUF_LEN (10 * sizeof(struct inotify_event) + NAME_MAX + 1)


int main(int argc, char *argv[])
{
    int inotifyFd, logFd, wd, j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    // inotifyインスタンスを作成
    inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        close(inotifyFd);
        errExit("inotify_init");
    
    }

    // 変更検知対象のディレクトリを指定
    // 実行ファイルと同一ディレクトリのファイルを監視する場合は、nameが入らない
    wd = inotify_add_watch(inotifyFd, WATCH_TARGET_DIR, IN_ALL_EVENTS);
    if (wd == -1) {
        errExit("inotify_add_watch");
    }
    printf("Watching %s using wd %d\n", "target.txt", wd);

    // ログ出力用のファイルを開く
    // 書き込む場合はファイル末尾へ追加する
    logFd = open(LOG_FILE, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (logFd == -1) {
        errExit("open");
    }



    // イベント処理用の無限ループ
    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            fatal("read() from inotify fd returned 0!");
        }

        if (numRead == -1) {
            errExit("read");
        }

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        // 読み込んだバッファの内容をinotify_event構造体にキャストして表示
        event = (struct inotify_event *) buf;
        writeToLog(logFd, event);
    }

    exit(EXIT_SUCCESS);
}