# configure.ac - Last modified: 07-Feb-2026 (kobayasy)
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

AC_INIT([syncTimeICOM], [1.3], [kobayasy@kobayasy.com], [syncTimeICOM])
AC_COPYRIGHT([Copyright @{:@C@:}@ 2024-2026 by Yuichi Kobayashi <kobayasy@kobayasy.com>])
AC_PREREQ(2.72)
AC_CONFIG_SRCDIR([syncTimeICOM.c])
AC_CONFIG_AUX_DIR([conf])

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
AS_VAR_IF([enableval], [yes],
   [AX_CFLAGS_WARN_ALL] )
AX_PTHREAD([LIBS="$PTHREAD_LIBS $LIBS" CFLAGS="$CFLAGS $PTHREAD_CFLAGS" CC="$PTHREAD_CC"])
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
MY_ARG_WITH([catdev], [DEVICE], [device filename for CAT @{:@default=/dev/cu.usbserial-110@:}@])
AS_VAR_SET_IF([withval],
   [AC_DEFINE_UNQUOTED([CATDEV], ["$withval"], [Deveice filename for CAT.])] )
MY_ARG_ENABLE([tzlocal], [enable], [adjust to local time])
AS_VAR_IF([enableval], [yes],
   [AC_DEFINE([TZLOCAL], [true], [Define to true if adjust to local time.])] )
MY_ARG_WITH([timeout], [MSEC], [timeout in msec for CAT])
AS_VAR_SET_IF([withval],
   [AC_DEFINE_UNQUOTED([POLL_TIMEOUT], [$withval], [Timeout in msec for CAT.])] )

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AH_TOP([#ifndef _INCLUDE_config_h
#define _INCLUDE_config_h])
AH_BOTTOM([#endif  /* #ifndef _INCLUDE_config_h */])
AX_APPEND_FLAG([-I.])
AC_OUTPUT
