#include "LCD.h"
#include <msp430.h>

#define IDLE 0
#define START 1
#define ACTIVE 2
#define STOPPED 3

int startTime = 0;

void chrono () {
    //turn on symbol to show mode

    //display 0
    LCD_WriteAll('0','0','0','0','0');
    LCD_WriteSingle('0', 6);

    int state = IDLE;
    
    //when start is pressed, 

    while (1){
        switch (state) {
            case IDLE:
                //if button pressed move to start state
                LCD_WriteAll('1','0','0','0','0');
                LCD_WriteSingle('0', 6);
                if (!(P2IN & BIT7)) {
                    state = START;
                }
                break;
            case START:
                // save timer start time
                startTime = TA0CCR0;
                state = ACTIVE;
                break;
            case ACTIVE:
            {
                // timer is counting
                int time = TA0CCR0 - startTime;
                char str[7];
                sprintf(str, "%d", time);
                LCD_WriteAll(str[0], str[1], str[2], str[3], str[4]);
                LCD_WriteSingle(str[5], 6);
                break;
            }
            case STOPPED:
                // timer is stopped showing what it stopped on
                break;
        }
    }
    
}