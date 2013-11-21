#include "time_constants.h"
#include "type_instrument.h"
#include "instrument_constants.h"

extern InstrumentConstants *ic;
extern TYPE_INSTRUMENT lmi;

TimeConstants *getDefaultTimeConstants(void) {
      static TimeConstants tc[] = {
        (TimeConstants){8, 0.0, 10.0, 1, 10.0, 480, 10},
        (TimeConstants){8, 0.0, 10.0, 1, 7.14, 4800, 10},
        (TimeConstants){8, 0.0, 10.0, 1, 7.14, 48000, 10},
        (TimeConstants){8, 0.0, 10.0, 1, 7.14, 480000, 10},
        (TimeConstants){8, 0.0, 10.0, 1, 7.14, 5000000, 10}
      };
      return tc;
}

TimeConstants getCurrentTimeConstants(void) {
    int i;
    TimeConstants *tc_arr = getDefaultTimeConstants();
    TimeConstants tc = tc_arr[ic->current_range - 1];
    for(i = 0; i < NUMBER_OF_FUNCTIONS; i++) {
        if(lmi.function[i].info.header.type == TIME_CONSTANTS) {
            tc.minTime *= lmi.function[i].info.time_constant.range[ic->current_range - 1];
            break;
        }
    }
    return tc;
}
