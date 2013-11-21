#include "doReading.h"
#include "instrument_constants.h"
#include "reading_calculation.h"

void doReading() {

    static InstrumentConstants ic = (InstrumentConstants){
        {
            // RangeFactors
            (RangeConstants){0, 45.47945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 100.0}, (RangeHysteresisTimeConstants){500000.0, 450.0}},
            (RangeConstants){0, 4.547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000.0}, (RangeHysteresisTimeConstants){164231.1, 4500.0}},
            (RangeConstants){0, 45.47945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 10000.0}, (RangeHysteresisTimeConstants){54063.7, 45000.0}},
            (RangeConstants){0.164, 4.547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 100000.0}, (RangeHysteresisTimeConstants){17797.4, 450000.0}},
            (RangeConstants){2.201, 0.4547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000000.0}, (RangeHysteresisTimeConstants){5858.8, 4500000.0}}

        },
        {
            // CalConstants
            1000, 1000, 1000, 1000, 1000
        },
        0.030, //chamber_output_low      
        0.000307, //chamber_output_high  
        1, //correction_factor           
        2.0, //over_range_voltage
        1,   // current_range
        1000 // id
    };

    doReadingCalculation(&ic);
}
