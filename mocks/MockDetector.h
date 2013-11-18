/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKDETECTOR_H
#define _MOCKDETECTOR_H

#include "Detector.h"

void MockDetector_Init(void);
void MockDetector_Destroy(void);
void MockDetector_Verify(void);




#define initializeDefaultDetector_Expect(Default_Detector) initializeDefaultDetector_CMockExpect(__LINE__, Default_Detector)
void initializeDefaultDetector_CMockExpect(UNITY_LINE_TYPE cmock_line, Detector* Default_Detector);
#define newDetector_ExpectAndReturn(model, Default_Detector, cmock_retval) newDetector_CMockExpectAndReturn(__LINE__, model, Default_Detector, cmock_retval)
void newDetector_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DETECTOR_NUMBER model, Detector* Default_Detector, Detector cmock_to_return);

#endif
