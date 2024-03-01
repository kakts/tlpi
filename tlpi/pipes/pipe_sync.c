/**
 * @file pipe_sync.c
 * 44.3 パイプによるプロセス同期
 * 
 * 実行例 子プロセスを3つ用意し、それぞれ4、2、6秒スリープさせる場合
 * ./pipe_sync 4 2 6
 */

#include "curr_time.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    // 同期に使用するパイプ
    int pfd[2];
    int j, dummy;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s sleep-time ...\n", argv[0]);
    }

    // 子プロセスは_exit()により終了するため、stdoutのバッファリング停止
    setbuf(stdout, NULL);

    printf("%s Parent started\n", currTime("%T"));

    if (pipe(pfd) == -1) {
        errExit("pipe");
    }

    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork %d", j);
        case 0:
            // 子プロセス
            // 読み取り口は不要
            if (close(pfd[0]) == -1) {
                errExit("close");
            }

            // 子プロセスは何らかの処理を実行し、その完了を親プロセスへ通知
            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));

            // 代替処理
            printf("%s Child %d (PID=%ld) closing pipe\n", currTime("%T"), j, (long) getpid());

            if (close(pfd[1]) == -1) {
                errExit("close");
            }

            // 必要に応じて子プロセスは他の処理も実行
            _exit(EXIT_SUCCESS);
        default:
            // 親プロセスは子プロセス作成を繰り返す
            break;
        }
    }

    // ここに到達するのは親プロセスのみ EOFを検知できるようにパイプの書き込み口をクローズ
    if (close(pfd[1]) == -1) {
        errExit("close");
    }

    // 親プロセスは処理を実行後、子プロセスと同期
    if (read(pfd[0], &dummy, 1) != 0) {
        fatal("parent didn't get EOF");
    }
    printf("%s Parent ready to go\n", currTime("%T"));

    exit(EXIT_SUCCESS);
}
