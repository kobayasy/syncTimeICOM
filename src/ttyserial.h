/* ttyserial.h - Last modified: 07-Feb-2026 (kobayasy)
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

#ifndef _INCLUDE_ttyserial_h
#define _INCLUDE_ttyserial_h

#include <termios.h>

typedef struct {
    int fd;
    struct termios attr;
} TTYSerial;

extern int ttyserial_new(TTYSerial *tty, int fd);
extern int ttyserial_flush(TTYSerial *tty);
extern void ttyserial_free(TTYSerial *tty);

#endif  /* #ifndef _INCLUDE_ttyserial_h */
