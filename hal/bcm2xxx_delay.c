#include "bcm2xxx_delay.h"


int delay_ms(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


int delay_us(long us)
{
    struct timespec ts;
    int res;

    if (us < 0)
    {
        errno = EINVAL;
        return -1;
    }
    
    unsigned int usecs = us%1000000;
    unsigned int wsecs = us / 1000000;

    ts.tv_sec = wsecs;
    ts.tv_nsec = usecs * 1000L;
    
	res = nanosleep(&ts, NULL);

    return res;
}

void delay_cycles(uint32_t cycles)
{
	for (int i = 0; i < cycles; ++i)
	{
		asm volatile ("");
	}
}
