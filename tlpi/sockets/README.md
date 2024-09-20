# sockets


## 57-3, 57-4 us_xfr simple unix domain socket server/client

docker コンテナに入る
```
make build
make run
```

- server を起動
serverプロセスによる出力をbというファイルにリダイレクトする。プロセスはバックグラウンドで実行
```
cd /tlpi/sockets

./us_xfr_sv > b &
```

serverプロセス起動により、/tmp/us_xfrにソケットファイルが作成されたことを確認
```
root@fe6941f05fdc:/tlpi/sockets# ls -lF /tmp/us_xfr
srwxr-xr-x 1 root root 0 Aug 23 15:59 /tmp/us_xfr=
```

- clientの実行

clientプロセスに渡す入力内容をaというファイルにリダイレクトする
ここでは /tlpi/sockets配下にあるcのファイルの内容を入力内容とする
```
cat *.c > a
```

clientプロセスを起動
```
./us_xfr_cl < a>

```

- 後処理
サーバ終了
```
kill %1
```

入力内容(a)と出力内容(b)が一致していることを確認
```
diff a b
```

## 57-6, 57-7 UNIXドメインのデータグラムソケットを使ったサーバ/クライアント
UNIXドメインのデータグラムソケットを使って、クライアントからメッセージを送信し
そのメッセージをサーバ側で受け取り、大文字にして返す
サーバ側で受信するメッセージのサイズは10バイトまで
それ以上を超えた場合は切り詰められ、先頭10文字のみ処理されて変換される

- サーバプロセスをバックグラウンドで実行
```bash
cd /tlpi/sockets
./ud_ucase_sv &
```

- クライアントから2つのメッセージを送信
```bash
./ud_ucase_cl hello world
Server received 5 bytes from /tmp/ud_ucase_cl.20150
Response 1: HELLO
Server received 5 bytes from /tmp/ud_ucase_cl.20150
Response 2: WORLD
```

- クライアントから制限の10バイトを超えるメッセージを送信
```bash
root@baa2ed11f645:/tlpi/sockets# ./ud_ucase_cl 'long message'
Server received 10 bytes from /tmp/ud_ucase_cl.17
Response 1: LONG MESSA
```

- バックグラウンドで実行されていたサーバプロセスを終了
```bash
kill %1
```

