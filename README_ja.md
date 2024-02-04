[インストール](#インストール)
|
[使い方](#使い方)

***
[![macOS](https://github.com/kobayasy/syncTimeICOM/workflows/macOS/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/build-macos.yml)
[![Linux](https://github.com/kobayasy/syncTimeICOM/workflows/Linux/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/build-linux.yml)
*|*
[![SAST](https://github.com/kobayasy/syncTimeICOM/workflows/SAST/badge.svg)](https://github.com/kobayasy/syncTimeICOM/actions/workflows/codeql-analysis.yml)

[syncTimeICOM] はオープンソースのICOM社製無線機の時刻設定コマンドです。
無線機をパソコンの時刻に合わせます。

## 動作環境

### PC側
POSIX準拠のOSならば動作すると思いますが、動作確認が出来ているのは macOS Sonoma 14.2.1 のみです。

### 無線機側
動作確認が出来ているのは IC-7300 のみですが、CAT の時刻設定プロトコルが互換ならば動作すると思います。
多分 IC-705 と IC-7100、IC-7600、IC-7610、IC-7850、IC-9700 にも対応してそうだけど...
確認環境が無いので不明です。
確認できる環境をお持ちでしたら情報を頂きたいです。
|無線機|OS|対応|CATデバイスファイル名|
|---|---|---|---|
|IC-705|?|?|-|
|IC-7100|?|?|-|
|IC-7300|macOS Sonoma 14.2.1|Yes|/dev/cu.usbserial-110|
|↑|Linux ?|?|-|
|IC-7600|?|?|-|
|IC-7610|?|?|-|
|IC-7850|?|?|-|
|IC-9700|?|?|-|

## インストール
1. 下記コマンドでビルドとインストールを実行します。
```
curl -LOJs https://github.com/kobayasy/syncTimeICOM/releases/download/1.0/syncTimeICOM-1.0.tar.gz
tar xzf syncTimeICOM-1.0.tar.gz
cd syncTimeICOM-1.0
./configure --prefix=$HOME
make install

```

2. PATH に ~/bin が含まれていない場合は追加してください。

## 使い方
1. 下記コマンドで使い方が表示されます。
```
syncTimeICOM --help

```

実行例↓
```
$ syncTimeICOM --help
syncTimeICOM 1.0

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
/dev/cu.usbserial-110 部分はPCが認識した無線機の CAT デバイスファイル名に置き換えて下さい。

```
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

```
./configure --help
```
--prefix= で実行ファイルのインストール先、
--with-catdev= で CAT デバイスファイル名、
--enable-tzlocal で設定時刻を現地時間
にそれぞれ設定出来ます。

実行例↓
```
$ ./configure --help
'configure' configures syncTimeICOM 1.0 to adapt to many kinds of systems.

Usage: ./configure [OPTION]... [VAR=VALUE]...

To assign environment variables (e.g., CC, CFLAGS...), specify them as
VAR=VALUE.  See below for descriptions of some of the useful variables.

Defaults for the options are specified in brackets.

Configuration:
  -h, --help              display this help and exit
      --help=short        display options specific to this package
      --help=recursive    display the short help of all the included packages
  -V, --version           display version information and exit
  -q, --quiet, --silent   do not print 'checking ...' messages
      --cache-file=FILE   cache test results in FILE [disabled]
  -C, --config-cache      alias for '--cache-file=config.cache'
  -n, --no-create         do not create output files
      --srcdir=DIR        find the sources in DIR [configure dir or '..']

Installation directories:
  --prefix=PREFIX         install architecture-independent files in PREFIX
                          [/usr/local]
  --exec-prefix=EPREFIX   install architecture-dependent files in EPREFIX
                          [PREFIX]

By default, 'make install' will install all the files in
'/usr/local/bin', '/usr/local/lib' etc.  You can specify
an installation prefix other than '/usr/local' using '--prefix',
for instance '--prefix=$HOME'.

For better control, use the options below.

Fine tuning of the installation directories:
  --bindir=DIR            user executables [EPREFIX/bin]
  --sbindir=DIR           system admin executables [EPREFIX/sbin]
  --libexecdir=DIR        program executables [EPREFIX/libexec]
  --sysconfdir=DIR        read-only single-machine data [PREFIX/etc]
  --sharedstatedir=DIR    modifiable architecture-independent data [PREFIX/com]
  --localstatedir=DIR     modifiable single-machine data [PREFIX/var]
  --runstatedir=DIR       modifiable per-process data [LOCALSTATEDIR/run]
  --libdir=DIR            object code libraries [EPREFIX/lib]
  --includedir=DIR        C header files [PREFIX/include]
  --oldincludedir=DIR     C header files for non-gcc [/usr/include]
  --datarootdir=DIR       read-only arch.-independent data root [PREFIX/share]
  --datadir=DIR           read-only architecture-independent data [DATAROOTDIR]
  --infodir=DIR           info documentation [DATAROOTDIR/info]
  --localedir=DIR         locale-dependent data [DATAROOTDIR/locale]
  --mandir=DIR            man documentation [DATAROOTDIR/man]
  --docdir=DIR            documentation root [DATAROOTDIR/doc/syncTimeICOM]
  --htmldir=DIR           html documentation [DOCDIR]
  --dvidir=DIR            dvi documentation [DOCDIR]
  --pdfdir=DIR            pdf documentation [DOCDIR]
  --psdir=DIR             ps documentation [DOCDIR]

System types:
  --build=BUILD     configure for building on BUILD [guessed]
  --host=HOST       cross-compile to build programs to run on HOST [BUILD]

Optional Features:
  --disable-option-checking  ignore unrecognized --enable/--with options
  --disable-FEATURE       do not include FEATURE (same as --enable-FEATURE=no)
  --enable-FEATURE[=ARG]  include FEATURE [ARG=yes]
  --enable-warnall        enable most reasonable warnings for compiler options
  --enable-tzlocal        adjust to local time

Optional Packages:
  --with-PACKAGE[=ARG]    use PACKAGE [ARG=yes]
  --without-PACKAGE       do not use PACKAGE (same as --with-PACKAGE=no)
  --with-catdev=DEVICE    device filename for CAT
                          (default=/dev/cu.usbserial-110)
  --with-timeout=MSEC     timeout in msec for CAT

Some influential environment variables:
  CC          C compiler command
  CFLAGS      C compiler flags
  LDFLAGS     linker flags, e.g. -L<lib dir> if you have libraries in a
              nonstandard directory <lib dir>
  LIBS        libraries to pass to the linker, e.g. -l<library>
  CPPFLAGS    (Objective) C/C++ preprocessor flags, e.g. -I<include dir> if
              you have headers in a nonstandard directory <include dir>
  CPP         C preprocessor

Report bugs to <kobayasy@kobayasy.com>.
$ 
```

[syncTimeICOM]: https://github.com/kobayasy/syncTimeICOM
