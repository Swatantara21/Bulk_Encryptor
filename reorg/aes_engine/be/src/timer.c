#include <stdlib.h>
#include <sys/time.h>

struct timeval _timers[8];

void sample_timer(int num_timer) 
{
	gettimeofday(&_timers[num_timer], NULL);
}

double get_millisecond_time(int num_timer) 
{
	double ret_val =  (((double)_timers[num_timer].tv_sec*1000.0) + ((double)_timers[num_timer].tv_usec/1000.0));
}

