#include "intrinsics.h"
#include "alarm.h"
void alarm () {
    //do something
    // Define an enum for the alarm process states
typedef enum {
    MINUTES,
    HOURS
} AlarmProcessStates;
AlarmProcessStates currentState = MINUTES;
int i = 0;

while(1){
            switch (currentState) {
            case MINUTES: // flash the minutes section on LCD
                __delay_cycles(150000);
                LCD_WriteAll('1', '2', '0', '0', ' ');
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000);
                LCD_WriteAll('1', '2', ' ', ' ', ' ');
                LCD_setAlarm();
                LCD_setColon();

                // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to hours state
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000);
                    if (!(P2IN & BIT6)) {
                        currentState = HOURS;
                    } // Check again if switch is still pressed}
                    }
            break;
            case HOURS:
                __delay_cycles(150000);
                LCD_WriteAll('1', '2', '0', '0', ' ');
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000);
                LCD_WriteAll(' ', ' ', '0', '0', ' ');
                LCD_setAlarm();
                LCD_setColon();
            // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to minutes state
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000);
                    if (!(P2IN & BIT6)) {
                        currentState = MINUTES;
                    } // Check again if switch is still pressed}
                    }
            break;
        }

}
    }
