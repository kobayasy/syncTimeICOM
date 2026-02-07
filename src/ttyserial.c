/* ttyserial.c - Last modified: 07-Feb-2026 (kobayasy)
 *
 * Copyright (C) 2024-2026 by Yuichi Kobayashi <kobayasy@kobayasy.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  /* #ifdef HAVE_CONFIG_H */

#include <termios.h>
#include "ttyserial.h"

int ttyserial_new(TTYSerial *tty, int fd) {
    int status = -1;
    struct termios attr;
    speed_t speed;

    tty->fd = fd;
    if (tcgetattr(tty->fd, &attr) == -1)
        goto error;
    tty->attr = attr;
    cfmakeraw(&attr);
/* ボーレイト設定 */
//    speed = B115200;  /* 115200bps */
//    speed =  B38400;  /*  38400bps */
//    speed =  B19200;  /*  19200bps */
    speed =   B9600;  /*   9600bps */
//    speed =   B2400;  /*   2400bps */
/* データビット幅 */
    attr.c_cflag &= ~CSIZE, attr.c_cflag |=  CS8;  /* 8bit */
//    attr.c_cflag &= ~CSIZE, attr.c_cflag |=  CS7;  /* 7bit */
//    attr.c_cflag &= ~CSIZE, attr.c_cflag |=  CS6;  /* 6bit */
//    attr.c_cflag &= ~CSIZE, attr.c_cflag |=  CS5;  /* 5bit */
/* パリティビット */
    attr.c_cflag &= ~PARENB;                           /* NONE */
//    attr.c_cflag |=  PARENB, attr.c_cflag &= ~PARODD;  /* EVEN */
//    attr.c_cflag |=  PARENB, attr.c_cflag  =  PARODD;  /* ODD */
/* ストップビット */
    attr.c_cflag &= ~CSTOPB;  /* 1bit */
//    attr.c_cflag |=  CSTOPB;  /* 2bit */
/* RTS/CTS制御 */
    attr.c_cflag &= ~CRTSCTS;  /* 無効 */
//    attr.c_cflag |=  CRTSCTS;  /* 有効 */
/* モデムライン制御 */
    attr.c_cflag |=  CLOCAL;  /* モデムライン制御: 無効 */
//    attr.c_cflag &= ~CLOCAL;  /* モデムライン制御: 有効 */
/* 受信動作 */
    attr.c_cflag |=  CREAD;  /* 有効 */
//    attr.c_cflag &= ~CREAD;  /* 無効 */
    if (cfsetispeed(&attr, speed) == -1 ||
        cfsetospeed(&attr, speed) == -1 ||
        tcsetattr(tty->fd, TCSAFLUSH, &attr) == -1 ) {
        tcsetattr(tty->fd, TCSANOW, &tty->attr);
        goto error;
    }
    status = 0;
 error:
    return status;
}

int ttyserial_flush(TTYSerial *tty) {
    int status = -1;

    if (tcdrain(tty->fd) != 0)
        goto error;
    status = 0;
error:
    return status;
}

void ttyserial_free(TTYSerial *tty) {
    ttyserial_flush(tty);
    tcsetattr(tty->fd, TCSANOW, &tty->attr);
}
