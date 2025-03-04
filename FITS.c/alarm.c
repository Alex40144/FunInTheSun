#include "intrinsics.h"
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
                // Show semicolon needed
                // show two full stops needed
                __delay_cycles(150000);
                LCD_WriteAll('1', '2', ' ', ' ', ' ');
                // Show semicolon needed
                // show two full stops needed

                // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to hours state
            break;
            case HOURS:
            // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to minutes state
            break;
        }

}
    }
