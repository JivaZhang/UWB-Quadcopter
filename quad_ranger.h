#ifndef QUAD_RANGER_H
#define QUAD_RANGER_H

#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

int timerValue,distance;
void quad_ranger_init();
void pinIntHandler(void);
void Timer0IntHandler(void);
void ResetListener(void);

#endif