#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "misc.h"

namespace base {

static void make_int(char* str, int val, int digits) {
    char* p = str + digits;
    for (int i = 0; i < digits; i++) {
        int d = val % 10;
        val /= 10;
        p--;
        *p = '0' + d;
    }
}

// format time
// inspired by the TPC-C benchmark from Evan Jones
// strftime is slow because it ends up consulting timezone info
// also, snprintf is slow
void time_now_str(char* now) {
    time_t seconds_since_epoch = time(nullptr);
    struct tm local_calendar;
    localtime_r(&seconds_since_epoch, &local_calendar);
    make_int(now, local_calendar.tm_year + 1900, 4);
    now[4] = '-';
    make_int(now + 5, local_calendar.tm_mon + 1, 2);
    now[7] = '-';
    make_int(now + 8, local_calendar.tm_mday, 2);
    now[10] = ' ';
    make_int(now + 11, local_calendar.tm_hour, 2);
    now[13] = ':';
    make_int(now + 14, local_calendar.tm_min, 2);
    now[16] = ':';
    make_int(now + 17, local_calendar.tm_sec, 2);
    now[19] = '.';
    timeval tv;
    gettimeofday(&tv, nullptr);
    make_int(now + 20, tv.tv_usec / 1000, 3);
    now[23] = '\0';
}

} // namespace base
