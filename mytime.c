#include <stdio.h>
#include <sys/time.h>

#include "mytime.h"

time_t get_current_time()
{
	struct timeval tv;
	if (0 == gettimeofday(&tv, NULL)) {
		return (tv.tv_sec * 1000000) + tv.tv_usec;
	}

	return 0;
}

time_t diff_time(time_t *start, time_t *end)
{
	return *end - *start;
}

