#include "LCD.h"

#define IDLE 0
#define START 0


void chrono () {
    //turn on symbol to show mode
    //display 0
    LCD_WriteAll('0','0','0','0','0');
    LCD_WriteSingle('0', 6)

    int state = 0
    
    //when start is pressed, 

    while (1){
        switch (state) {
            IDLE:
                //if button pressed move to start state
            break;
        }
    }
    
}