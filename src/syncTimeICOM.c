/* syncTimeICOM.c - Last modified: 03-Feb-2024 (kobayasy)
 *
 * Copyright (C) 2024 by Yuichi Kobayashi <kobayasy@kobayasy.com>
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "ttyserial.h"
#include "tpbar.h"

#ifndef PACKAGE_STRING
#define PACKAGE_STRING "syncTImeICOM"
#endif  /* #ifndef PACKAGE_STRING */
#ifndef PACKAGE_TARNAME
#define PACKAGE_TARNAME "syncTImeICOM"
#endif  /* #ifndef PACKAGE_TARNAME */
#ifndef CATDEV
#define CATDEV "/dev/cu.usbserial-110"
#endif  /* #ifndef CATDEV */
#ifndef TZLOCAL
#define TZLOCAL false
#endif  /* #ifndef TZLOCAL */

static signed long tsdiff(const struct timespec *now, const struct timespec *last) {
    signed long sec, nsec;

    nsec = (signed long)now->tv_nsec - last->tv_nsec;
    sec = (signed long)now->tv_sec - last->tv_sec;
    if (nsec < 0)
        nsec += 1000000000, --sec;
    else if (nsec > 1000000000)
        nsec -= 1000000000, ++sec;
    return sec * 1000000 + nsec / 1000;
}

static const uint8_t BCD[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99
};
static const uint8_t GetR[] = {
    0xfe, 0xfe, 0xe0, 0x94, 0xfb, 0xfd
};

static int syncTimeICOM(TTYSerial *tty, const struct timespec *ts, bool tzlocal) {
    int status = INT_MIN;
    uint8_t SetT[] = {
        0xfe, 0xfe, 0x94, 0xe0, 0x1a, 0x05, 0x00, 0x95,
        0x00, 0x00, 0xfd
    };
    uint8_t SetD[] = {
        0xfe, 0xfe, 0x94, 0xe0, 0x1a, 0x05, 0x00, 0x94,
        0x00, 0x00, 0x00, 0x00, 0xfd
    };
    uint8_t SetZ[] = {
        0xfe, 0xfe, 0x94, 0xe0, 0x1a, 0x05, 0x00, 0x96,
        0x00, 0x00, 0x00, 0xfd
    };
    struct tm *(*time_r)(const time_t *clock, struct tm *result);
    struct tm tm;
    uint8_t buffer[sizeof(GetR)];

    time_r = tzlocal ? localtime_r : gmtime_r;
    if (time_r(&ts->tv_sec, &tm) == NULL) {
        status = -1;
        goto error;
    }
    SetT[ 9] = BCD[tm.tm_min % 100], tm.tm_min /= 100;
    SetT[ 8] = BCD[tm.tm_hour % 100], tm.tm_hour /= 100;
    if (write_size(tty->fd, SetT, sizeof(SetT)) != sizeof(SetT) ||
        read_size(tty->fd, buffer, sizeof(GetR)) != sizeof(GetR) ||
        memcmp(buffer, GetR, sizeof(GetR)) ) {
        status = -1;
        goto error;
    }
    SetD[11] = BCD[tm.tm_mday % 100], tm.tm_mday /= 100;
    ++tm.tm_mon;
    SetD[10] = BCD[tm.tm_mon % 100], tm.tm_mon  /= 100;
    tm.tm_year += 1900;
    SetD[ 9] = BCD[tm.tm_year % 100], tm.tm_year /= 100;
    SetD[ 8] = BCD[tm.tm_year % 100], tm.tm_year /= 100;
    if (write_size(tty->fd, SetD, sizeof(SetD)) != sizeof(SetD) ||
        read_size(tty->fd, buffer, sizeof(GetR)) != sizeof(GetR) ||
        memcmp(buffer, GetR, sizeof(GetR)) ) {
        status = -1;
        goto error;
    }
    if (tm.tm_gmtoff < 0)
        SetZ[10] = 0x01, tm.tm_gmtoff /= -60;
    else
        SetZ[10] = 0x00, tm.tm_gmtoff /=  60;
    SetZ[ 9] = BCD[tm.tm_gmtoff % 60], tm.tm_gmtoff /= 60;
    SetZ[ 8] = BCD[tm.tm_gmtoff % 100], tm.tm_gmtoff /= 100;
    if (write_size(tty->fd, SetZ, sizeof(SetZ)) != sizeof(SetZ) ||
        read_size(tty->fd, buffer, sizeof(GetR)) != sizeof(GetR) ||
        memcmp(buffer, GetR, sizeof(GetR)) ) {
        status = -1;
        goto error;
    }
    status = 0;
error:
    return status;
}

static int powerICOM(TTYSerial *tty, bool power) {
    int status = INT_MIN;
    uint8_t pwrOnOff[] = {
        0xfe, 0xfe, 0x94, 0xe0, 0x18, 0x00, 0xfd
    };
    signed long t;
    struct timespec tslast, tsnow;
    uint8_t buffer[sizeof(GetR)];

    if (power)
        pwrOnOff[ 5] = 0x01, t = 1300;
    else
        pwrOnOff[ 5] = 0x00, t =    0;
    if (t > 0) {
        if (clock_gettime(CLOCK_REALTIME, &tslast) == -1) {
            status = -1;
            goto error;
        }
        do {
            if (write_size(tty->fd, pwrOnOff, 1) != 1) {
                status = -1;
                goto error;
            }
            ONERR(ttyserial_flush(tty), -1);
            if (clock_gettime(CLOCK_REALTIME, &tsnow) == -1) {
                status = -1;
                goto error;
            }
        } while (tsdiff(&tsnow, &tslast) < t);
    }
    if (write_size(tty->fd, pwrOnOff, sizeof(pwrOnOff)) != sizeof(pwrOnOff) ||
        read_size(tty->fd, buffer, sizeof(GetR)) != sizeof(GetR) ||
        memcmp(buffer, GetR, sizeof(GetR)) ) {
        status = -1;
        goto error;
    }
    status = 0;
error:
    return status;
}

typedef struct {
    struct timespec ts;
    bool tzlocal;
    int row;
    TPBAR tpbar;
    int status;
} INFO_PARAM;
static void *info_thread(void *data) {
    INFO_PARAM *param = data;
    struct tm *(*time_r)(const time_t *clock, struct tm *result);
    struct timespec tsnow, tslast;
    struct tm tm;
    char text[1024];
    char buffer[1024], *s;

    time_r = param->tzlocal ? localtime_r : gmtime_r;
    if (clock_gettime(CLOCK_REALTIME, &tsnow) == -1) {
        param->status = -1;
        goto error;
    }
    tslast = tsnow;
    while (tsdiff(&param->ts, &tsnow) > 0) {
        if (usleep(250000) == -1 ||
            clock_gettime(CLOCK_REALTIME, &tsnow) == -1 ||
            time_r(&param->ts.tv_sec, &tm) == NULL ) {
            param->status = -1;
            goto error;
        }
        sprintf(text, "[ Waiting for %2ld seconds until exactly %02d:%02d %s ]",
                tsdiff(&param->ts, &tsnow) / 1000000, tm.tm_hour, tm.tm_min, tm.tm_zone );
        s = buffer;
        s += tpbar_setrow(s, param->row, &param->tpbar);
        s += tpbar_printf(s, tsdiff(&tsnow, &tslast), tsdiff(&param->ts, &tslast), &param->tpbar, text);
        if (write(STDOUT_FILENO, buffer, s - buffer) == -1) {
            param->status = -1;
            goto error;
        }
    }
    param->status = 0;
error:
    return NULL;
}

static void next0sec(struct timespec *ts) {
    time_t sec;

    sec = ts->tv_sec % 60, ts->tv_sec /= 60;
    if (ts->tv_nsec > 0 || sec > 0)
        ++ts->tv_sec;
    ts->tv_nsec = 0, ts->tv_sec *= 60;
}

typedef struct {
    bool usage;
    const char *catdev;
    bool tzlocal;
} OPT;

static int run(OPT *opt) {
    int status = INT_MIN;
    int fd = -1;
    TTYSerial tty;
    INFO_PARAM info_param = {
        .tzlocal = opt->tzlocal,
        .status = INT_MIN
    };
    struct tm *(*time_r)(const time_t *clock, struct tm *result);
    int row;
    char buffer[1024], *s;
    pthread_t tid;
    bool info;
    struct timespec ts;
    struct tm tm;

    fd = open(opt->catdev, O_RDWR);
    if (fd == -1) {
        status = -1;
        goto error;
    }
    ONERR(ttyserial_new(&tty, fd), -1);
    time_r = info_param.tzlocal ? localtime_r : gmtime_r;
    if (clock_gettime(CLOCK_REALTIME, &info_param.ts) == -1) {
        status = -1;
        goto error;
    }
    info_param.ts.tv_sec += 10, next0sec(&info_param.ts);
    tpbar_init(&info_param.tpbar);
    row = tpbar_getrow(0, &info_param.tpbar);
    powerICOM(&tty, true);
    s = buffer;
    s += tpbar_setrow(s, row++, &info_param.tpbar);
    s += sprintf(s, "Power on");
    write(STDOUT_FILENO, buffer, s - buffer);
    info_param.row = row++;
    info = pthread_create(&tid, NULL, info_thread, &info_param) == 0;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1 ||
        usleep(tsdiff(&info_param.ts, &ts)) == -1 ) {
        status = -1;
        goto error;
    }
    status = syncTimeICOM(&tty, &info_param.ts, info_param.tzlocal);
    if (time_r(&info_param.ts.tv_sec, &tm) == NULL) {
        status = -1;
        goto error;
    }
    s = buffer;
    s += tpbar_setrow(s, row++, &info_param.tpbar);
    s += sprintf(s, "Adjusted to %02d:%02d %s", tm.tm_hour, tm.tm_min, tm.tm_zone);
    write(STDOUT_FILENO, buffer, s - buffer);
    powerICOM(&tty, false);
    s = buffer;
    s += tpbar_setrow(s, row++, &info_param.tpbar);
    s += sprintf(s, "Power off");
    write(STDOUT_FILENO, buffer, s - buffer);
    ttyserial_free(&tty);
    if (info) {
        pthread_join(tid, NULL);
        ONERR(info_param.status, -1);
    }
    s = buffer;
    s += tpbar_setrow(s, INT_MAX, &info_param.tpbar);
    write(STDOUT_FILENO, buffer, s - buffer);
    status = 0;
error:
    if (fd != -1)
        close(fd);
    return status;
}

static int parse_arg(int argc, char *argv[], OPT *opt) {
    int status = INT_MIN;

    while (*++argv != NULL)
        if (!strcmp(*argv, "--help"))
            opt->usage = true;
        else if (!strcmp(*argv, "-l") || !strcmp(*argv, "--local"))
            opt->tzlocal = true;
        else if (!strcmp(*argv, "-u") || !strcmp(*argv, "--utc"))
            opt->tzlocal = false;
        else if (**argv == '-')
            goto error;
        else
            opt->catdev = *argv;
    status = 0;
error:
    return status;
}

static int usage(FILE *fp) {
    int status = INT_MIN;

    fprintf(fp, PACKAGE_STRING"\n"
                "\n"
                "Usage: "PACKAGE_TARNAME" [-u|-l] [--utc|--local] [CATDEV]\n"
                "       "PACKAGE_TARNAME" --help\n"
                "\n"
                "CATDEV         device filename for CAT (default: "CATDEV")\n"
                "\n"
                "runmode\n"
#if TZLOCAL
                "  -u, --utc    adjust to UTC\n"
                "  -l, --local  adjust to local time (default)\n"
#else
                "  -u, --utc    adjust to UTC (default)\n"
                "  -l, --local  adjust to local time\n"
#endif
                "\n"
                "subcommand\n"
                "  --help       show this help\n"
                "\n" );
    status = 0;
    return status;
}

int main(int argc, char *argv[]) {
    int status = INT_MIN;
    OPT opt = {
        .usage = false,
        .catdev = CATDEV,
        .tzlocal = TZLOCAL
    };

    ONERR(parse_arg(argc, argv, &opt), -1);
    status = opt.usage ? usage(stdout) : run(&opt);
error:
    if (status < 0)
        fprintf(stderr, "Error\n");
    return ISERR(status) ? 1 : 0;
}
