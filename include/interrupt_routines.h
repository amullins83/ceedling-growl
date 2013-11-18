#ifndef INTERRUPT_ROUTINES_H
#define INTERRUPT_ROUTINES_H

#include "globals_29307.h"
//**************************************************************************************************
//**************************************************************************************************
//* INTERRUPT ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_1_VECTOR, ipl6) Timer1Handler(void)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Uses Timer 1
//*

void __ISR(_TIMER_1_VECTOR, ipl6) Timer1Handler(void);
//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl6) OC1_IntHandler(void)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Uses Timer 2 and Timer 3
//*

void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl6) OC1_IntHandler(void);
//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_3_VECTOR, ipl6) _T3Interrupt(void)
//*
//* Description:
//*     Timer ISR, used to update application state. If no transfers are pending new input request
//*     is scheduled.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Timer 3 (used by output compare for audio)
//*

void __ISR(_TIMER_3_VECTOR, ipl6) _T3Interrupt(void);
//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_4_VECTOR, ipl5) _T4Interrupt(void)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Timer 4 (used by keyboard)
//*

void __ISR(_TIMER_4_VECTOR, ipl5) _T4Interrupt(void);
//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_5_VECTOR, ipl7) Timer5Handler(void)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Timer 5 (used by electrometer sampling)
//*

void __ISR(_TIMER_5_VECTOR, ipl6) Timer5Handler(void);
//*
//**************************************************************************************************
//**************************************************************************************************
//* END INTERRUPT ROUTINES
//**************************************************************************************************
//**************************************************************************************************


#endif
