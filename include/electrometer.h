#ifndef ELECTROMETER_H
#define ELECTROMETER_H

#include "GenericTypeDefs.h"
#define Electro_Default                 0x0E
#define Electro_Clear_Range             0xE8
#define Electro_Gain_None               0x07

#define Electro_Gain_0                  0x06
#define Electro_Gain_1                  0x05
#define Electro_Gain_2                  0x03

#define Electro_Hi                      0x10
#define Electro_Lo                      0x00

#define Range_1                         (Electro_Gain_0 | Electro_Lo)
#define Range_10                        (Electro_Gain_1 | Electro_Lo)
#define Range_100                       (Electro_Gain_0 | Electro_Hi)
#define Range_1K                        (Electro_Gain_1 | Electro_Hi)
#define Range_10K                       (Electro_Gain_2 | Electro_Hi)

typedef struct {
    short multiplier;
    float conversion;      // n1 Assists with calculating a new range based on the user's currently selected units.
    float result;          // n1 Conversion sets the unit conversion from default to current unit, multiplier selects
                           //    prefix (n, m, k, unity, M, G, etc.), and result stores the max rate for this range in the user units.
} RelativeAdjustment;

RelativeAdjustment GetRelativeAdjustment(BYTE range);

void Change_Range(BYTE new_range);
void Update_Range(void);
float GetRateResult(void);                 // n1 Extracted to facilitate unit conversions.

#endif
