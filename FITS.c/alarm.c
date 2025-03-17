#include "intrinsics.h"
#include "alarm.h"
#include "time.h"

int currentAlarmHoursMins[] = {12,0};

int Alarm_getAlarmHours()
{
    return currentAlarmHoursMins[0];
}

int Alarm_getAlarmMins()
{
    return currentAlarmHoursMins[1];
}

void setLCDAlarmHours(){
    int hours = currentAlarmHoursMins[0];
    int hourDigitOne = hours / 10;
    int hourDigitTwo = hours % 10;
    LCD_WriteSingle(hourDigitOne,1);
    LCD_WriteSingle(hourDigitTwo,2);
}

void setLCDAlarmMins(){
    int mins = currentAlarmHoursMins[1];
    int minDigitOne = mins / 10;
    int minDigitTwo = mins % 10;
    LCD_WriteSingle(minDigitOne,3);
    LCD_WriteSingle(minDigitTwo,4);
}

void setLCDAlarmHoursNull(){
    LCD_WriteSingle(' ',1);
    LCD_WriteSingle(' ',2);
}

void setLCDAlarmMinsNull(){
    LCD_WriteSingle(' ',3);
    LCD_WriteSingle(' ',4);
}

void alarm () {
    //do something
    // Define an enum for the alarm process states
typedef enum {
    MINUTES,
    HOURS
} AlarmProcessStates;
AlarmProcessStates currentState = MINUTES;


while(1){
            switch (currentState) {
            case MINUTES: // flash the minutes section on LCD
                __delay_cycles(100000);
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000);
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDDotW();
                setLCDAlarmMinsNull();
                LCD_setAlarm();
                LCD_setColon();

                // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to hours state
                __delay_cycles(50000);
                if (!(P1IN & BIT4)) {
                    __delay_cycles(150000);
                    if (!(P1IN & BIT4)) {
                        currentState = HOURS;
                    } // Check again if switch is still pressed}
                    }
                
                // check if START/STOP is pressed to increment alarm time
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000);
                    while (!(P2IN & BIT6)) {
                        currentAlarmHoursMins[1] = currentAlarmHoursMins[1] + 1;
                        if (currentAlarmHoursMins[1] == 60) // check if passed 60 minute clock limit
                        {
                            currentAlarmHoursMins[1] = 0;
                        }
                        setLCDAlarmMins();
                        __delay_cycles(350000);
                    } // Check again if switch is still pressed}
                }
            break;
            case HOURS:
                __delay_cycles(100000);
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000);
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDDotW();
                setLCDAlarmHoursNull();
                LCD_setAlarm();
                LCD_setColon();

            // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to minutes state
                __delay_cycles(50000);
                if (!(P1IN & BIT4)) {
                    __delay_cycles(150000);
                    if (!(P1IN & BIT4)) {
                        currentState = MINUTES;
                        __delay_cycles(100000);
                    } // Check again if switch is still pressed}
                    }

                // check if START/STOP is pressed to increment alarm time
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000);
                    while (!(P2IN & BIT6)) {
                        currentAlarmHoursMins[0] = currentAlarmHoursMins[0] + 1;
                        if (currentAlarmHoursMins[0] == 24) // check if passed 24 hour clock limit
                        {
                            currentAlarmHoursMins[0] = 0;
                        }
                        setLCDAlarmHours();
                        __delay_cycles(350000);
                    } // Check again if switch is still pressed}
                }
            break;
        }

}
    }
