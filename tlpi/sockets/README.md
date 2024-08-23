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