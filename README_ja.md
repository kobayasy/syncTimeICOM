<!--
README_ja.md - Last modified: 08-Feb-2026 (kobayasy)
-->

[
	[インストール](#インストール) |
	[使い方](#使い方)
]

---

[![macOS](https://github.com/kobayasy/syncTimeICOM/workflows/macOS/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/build-macos.yml)
[![Linux](https://github.com/kobayasy/syncTimeICOM/workflows/Linux/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/build-linux.yml)
<sup>|</sup>
[![SAST](https://github.com/kobayasy/syncTimeICOM/workflows/SAST/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/codeql-analysis.yml)

オープンソースのICOM社製無線機の時刻設定コマンドです。
無線機をパソコンの時刻に合わせます。

### 動作環境

#### PC側
POSIX準拠のOSならば動作すると思いますが、動作確認が出来ているのは macOS のみです。

#### 無線機側
動作確認が出来ているのは IC-7300 のみですが、CAT の時刻設定プロトコルが互換ならば動作すると思います。
多分 IC-705 と IC-7100、IC-7600、IC-7610、IC-7850、IC-9700 にも対応してそうだけど...
確認環境が無いので不明です。

### インストール
1. 下記コマンドでビルドとインストールを実行します。
```sh
curl -LOJs https://github.com/kobayasy/syncTimeICOM/releases/download/1.3/syncTimeICOM-1.3.tar.gz
tar xzf syncTimeICOM-1.3.tar.gz
cd syncTimeICOM-1.3
./configure --prefix=$HOME
make install
```

2. PATH に `~/bin` が含まれていない場合は追加してください。

### 使い方
1. 下記コマンドで使い方が表示されます。
```sh
syncTimeICOM --help
```

実行例↓
```
$ syncTimeICOM --help
syncTimeICOM 1.3

Usage: syncTimeICOM [-u|-l] [--utc|--local] [CATDEV]
       syncTimeICOM --help

CATDEV         device filename for CAT (default: /dev/cu.usbserial-110)

runmode
  -u, --utc    adjust to UTC (default)
  -l, --local  adjust to local time

subcommand
  --help       show this help

$ 
```

2. 無線機をPCに繋いで、電源を切った状態で下記コマンドを実行して下さい。
`/dev/cu.usbserial-110` 部分はPCが認識した無線機の CAT デバイスファイル名に置き換えて下さい。

```sh
syncTimeICOM --utc /dev/cu.usbserial-110
```
無線機の電源が入る
→
次の丁度0秒のタイミングを待つ
→
無線機の時刻が設定される
→
無線機の電源が切れる
の順番に処理が進んでコマンドプロンプトに帰ってくれば成功です。

実行例↓
```
$ syncTimeICOM --utc /dev/cu.usbserial-110
Power on
[ Waiting for  0 seconds until exactly 09:18 UTC ]
Adjusted to 09:18 UTC
Power off
$ 
```

オプション省略時は使い方表示の default が有効になります。
default は [インストール](#インストール) 時の configure オプションで変更出来ます。
上記で動作確認出来た設定を configure オプションに反映して [インストール](#インストール) し直すと、以降オプションなしで実行出来て便利です。
configure のオプションは下記コマンドで表示されます。

```sh
./configure --help
```
`--prefix=` で実行ファイルのインストール先、`--with-catdev=` で CAT デバイスファイル名、`--enable-tzlocal` で設定時刻を現地時間 にそれぞれ設定出来ます。
