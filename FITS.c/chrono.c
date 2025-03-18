#include "LCD.h"
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#define IDLE 0
#define START 1
#define ACTIVE 2
#define STOPPED 3


void chrono () {
    //turn on symbol to show mode

    //display 0
    LCD_WriteAll('0','0','0','0','0', '0');

    int state = IDLE;
    int startTime = 0;
    
    //when start is pressed, 

    while (1){
        switch (state) {
            case IDLE:
                //if button pressed move to start state
                LCD_WriteAll('1','0','0','0','0', '0');
                LCD_setStopWatch();
                if (getSTARTSTOP()) {
                    state = START;
                    clearSTARTSTOP();
                }
                break;
            case START:
                // save timer start time
                startTime = getGlobalTimer();
                state = ACTIVE;
                break;
            case ACTIVE:
            {
                uint16_t time = getGlobalTimer() - startTime;
                // Calculate minutes, seconds, and centiseconds
                int minutes = time / 6000;               // 1 minute = 6000 centiseconds
                int seconds = (time % 6000) / 100;       // 1 second = 100 centiseconds
                int centiseconds = time % 100;

                char digits[6];
                // Extract digits for mm:ss:SS format
                digits[0] = minutes / 10;       // First digit of minutes
                digits[1] = minutes % 10;       // Second digit of minutes
                digits[2] = seconds / 10;       // First digit of seconds
                digits[3] = seconds % 10;       // Second digit of seconds
                digits[4] = centiseconds / 10;  // First digit of centiseconds
                digits[5] = centiseconds % 10;  // Second ditig of centiseconds
                LCD_WriteAll(digits[0], digits[1], digits[2], digits[3], digits[4], digits[5]);
                LCD_setColon();
                LCD_setDecimalTwo();

                if (getSTARTSTOP()) {
                    state = STOPPED;
                    clearSTARTSTOP();
                }
                break;
            }
            case STOPPED:
                //reset.
                break;
        }
    }
    
}
