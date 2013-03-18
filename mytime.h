#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <sys/time.h>

extern time_t get_current_time();
extern time_t diff_time(time_t *start, time_t *end);

#endif
