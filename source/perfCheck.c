#include "perfCheck.h"
#include <time.h>

time_t perfCheck(Task t, unsigned int loops) {
    time_t beginTime, endTime;
    time(&beginTime);
    while(loops--)
        t();
    time(&endTime);
    return difftime(endTime, beginTime);
}
