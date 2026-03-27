# configure.ac - Last modified: 28-Mar-2026 (kobayasy)
#
# Copyright (C) 2024-2026 by Yuichi Kobayashi <kobayasy@kobayasy.com>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

AC_INIT([syncTimeICOM], [1.4], [kobayasy@kobayasy.com], [syncTimeICOM])
AC_COPYRIGHT([Copyright @{:@C@:}@ 2024-2026 by Yuichi Kobayashi <kobayasy@kobayasy.com>])
AC_PREREQ([2.72])
AC_CONFIG_SRCDIR([syncTimeICOM.c])
AC_CONFIG_AUX_DIR([conf])

AC_DEFUN([MY_CATDEV_DEFAULT], [/dev/cu.usbserial-110])
AC_DEFUN([MY_TIMEOUT_DEFAULT], [disabled])

AC_DEFUN([MY_ARG_ENABLE],
   [AC_ARG_ENABLE([$1], [AS_HELP_STRING([--$2-$1], [$3])],
       [AS_CASE([$enableval],
         [yes|no],
           [],
           [AC_MSG_WARN([invalid option: --enable-$1=$enableval])
            AS_UNSET([enableval]) ] )],
       [AS_UNSET([enableval])] )] )
AC_DEFUN([MY_ARG_WITH],
   [AC_ARG_WITH([$1], [AS_HELP_STRING([--with-$1=$2], [$3])],
       [AS_CASE([$withval],
         [yes|no],
           [AC_MSG_WARN([invalid option: --with-$1=$withval])
            AS_UNSET([withval]) ] )],
       [AS_UNSET([withval])] )] )

AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_INSTALL

MY_ARG_ENABLE([warnall], [enable], [enable most reasonable warnings for compiler options])
AS_VAR_IF([enable_warnall], [yes],
   [AX_CFLAGS_WARN_ALL] )
AX_PTHREAD(
   [AS_VAR_SET([CC], ["$PTHREAD_CC"])
    AX_APPEND_FLAG([$PTHREAD_CFLAGS])
    AS_VAR_SET([LIBS], ["$PTHREAD_LIBS $LIBS"]) ] )
AC_CHECK_FUNC([clock_gettime],
   [],
   [AC_CHECK_LIB([rt], [clock_gettime])] )
AC_CHECK_HEADERS([termcap.h],
   [AC_CHECK_FUNC([tgetent],
       [AC_DEFINE([HAVE_TGETENT], [1], [Define to 1 if you have the 'tgetent' function.])],
       [AC_SEARCH_LIBS([tgetent], [tinfo ncurses],
           [AC_DEFINE([HAVE_TGETENT], [1])] )] )] )
AC_SYS_POSIX_TERMIOS
AS_VAR_IF([ac_cv_sys_posix_termios], [no],
   [AC_MSG_ERROR([missing function: POSIX termios.])] )
MY_ARG_WITH([catdev], [DEVICE], [device filename for CAT @<:@]MY_CATDEV_DEFAULT[@:>@])
AS_VAR_SET_IF([with_catdev],
   [AC_DEFINE_UNQUOTED([CATDEV], ["$with_catdev"], [Deveice filename for CAT.])] )
MY_ARG_ENABLE([tzlocal], [enable], [adjust to local time])
AS_VAR_IF([enable_tzlocal], [yes],
   [AC_DEFINE([TZLOCAL], [true], [Define to true if adjust to local time.])] )
MY_ARG_WITH([timeout], [SECS], [communication timeout @<:@]MY_TIMEOUT_DEFAULT[@:>@])
AS_VAR_SET_IF([with_timeout],
   [AC_DEFINE_UNQUOTED([POLL_TIMEOUT], [$with_timeout], [Communication timeout in seconds.])] )
AC_CANONICAL_HOST
MY_ARG_ENABLE([universal2], [enable], [build universal binary @{:@arm64+x86_64@:}@ on macOS])
AS_VAR_IF([enable_universal2], [yes],
   [AS_CASE([$host_os],
     [darwin*],
       [AX_APPEND_FLAG([-arch arm64])
        AX_APPEND_FLAG([-arch x86_64]) ],
       [AC_MSG_ERROR([--enable-universal2 is supported only on macOS])] )] )

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AH_TOP([@%:@ifndef _INCLUDE_config_h
@%:@define _INCLUDE_config_h])
AH_BOTTOM([@%:@endif  /* @%:@ifndef _INCLUDE_config_h */])
AX_APPEND_FLAG([-I.], [CPPFLAGS])
AC_OUTPUT
