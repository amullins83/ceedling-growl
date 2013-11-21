#ifndef PERF_CHECK_H
#define PERF_CHECK_H

    #include <time.h>

    typedef void (*Task)(void);
    time_t perfCheck(Task t, unsigned int loops);

#endif
