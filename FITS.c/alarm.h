#include <msp430.h> // generic board control library
#include "LCD.h"    // for controlling the onboard LCD
void alarm();       // main loop for the alarm configuration mode of the stop watch

int Alarm_getAlarmMins();   // returns the current mins value for the alarm
int Alarm_getAlarmHours();  // returns the current hours value for the alarm
