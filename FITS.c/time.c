//Hour, Min, Weekday, Date(Day), Monrth
//Weekdays0 = Monday, 6 = Sunday
#include "intrinsics.h"
#include <stdio.h>
#include <msp430.h>

int process_switched = 0;
// Hours, Mins, Seconds, DOTW, Date, Month
int currentDayTime[] = {1,11,0 ,3,15,9};

enum States{STATE_MINS,STATE_HOURS,STATE_DOTW,STATE_DATE,STATE_MONTH}Current_State;

void setTimeSwitch(){
    process_switched = 1;
}

void clearTimeSwitch(){
    process_switched = 0;
}

int check_SW3(){
    if (!(P1IN & BIT4)) {
        __delay_cycles(400000);
        if (!(P1IN & BIT4)) {
            return 1;
        } // Check again if switch is still pressed}
    }
    return 0;
}

int setLCDHours(){
    int hours = currentDayTime[0];
    int hourDigitOne = hours / 10;
    int hourDigitTwo = hours % 10;
    LCD_WriteSingle(hourDigitOne,1);
    LCD_WriteSingle(hourDigitTwo,2);
}

int setLCDMins(){
    int mins = currentDayTime[1];
    int minDigitOne = mins / 10;
    int minDigitTwo = mins % 10;
    LCD_WriteSingle(minDigitOne,3);
    LCD_WriteSingle(minDigitTwo,4);
}

int setLCDDotW(){
    switch(currentDayTime[3]){
        case 0:
            LCD_WriteSingle('M',5);
            LCD_WriteSingle('O',6);
            break;
        case 1:
            LCD_WriteSingle('T',5);
            LCD_WriteSingle('U',6);
            break;
        case 2:
            LCD_WriteSingle('W',5);
            LCD_WriteSingle('E',6);
            break;
        case 3:
            LCD_WriteSingle('T',5);
            LCD_WriteSingle('H',6);
            break;
        case 4:
            LCD_WriteSingle('F',5);
            LCD_WriteSingle('R',6);
            break;
        case 5:
            LCD_WriteSingle('S',5);
            LCD_WriteSingle('A',6);
            break;
        case 6:
            LCD_WriteSingle('S',5);
            LCD_WriteSingle('U',6);
            break;
        default:
            LCD_WriteSingle('X',5);
            LCD_WriteSingle('X',6);
            break;
    }
}

int setLCDDate(){
    int Date = currentDayTime[4];
    int dateDigitOne = Date / 10;
    int dateDigitTwo = Date % 10;
    LCD_WriteSingle(dateDigitOne,1);
    LCD_WriteSingle(dateDigitTwo,2);
}

int setLCDMonth(){
    int Month = currentDayTime[5];
    int monthDigitOne = Month / 10;
    int monthDigitTwo = Month % 10;
    LCD_WriteSingle(monthDigitOne,3);
    LCD_WriteSingle(monthDigitTwo,4);
}

int clkIncrement(){
    //Increment seconds
    currentDayTime[2]++;
    //60 seconds
    if(currentDayTime[2]==60){
        //reset seconds
        currentDayTime[2] = 0;
        //increment mins
        currentDayTime[1]++;
        //60 mins
        if(currentDayTime[1]==60){
            //reset mins
            currentDayTime[1] =0;
            //increment hours
            currentDayTime[0]++;
            //24 hours
            if(currentDayTime[0]==24){
                //reset hours
                currentDayTime[0] = 0;
                //increment DOTW
                currentDayTime[3]++;
                currentDayTime[3] = currentDayTime[3] %7;
                //increment date
                currentDayTime[4]++;
                //check full month
                switch(currentDayTime[5]){
                    case 1:
                    case 3:
                    case 5:
                    case 7:
                    case 8:
                    case 10:
                    case 12:
                        if(currentDayTime[4]>31){
                            currentDayTime[4] = 1;
                            currentDayTime[5]++;
                        }
                        break;
                    case 2:
                        if(currentDayTime[4]>28){
                            currentDayTime[4] = 1;
                            currentDayTime[5]++;
                        }
                        break;
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        if(currentDayTime[4]>30){
                            currentDayTime[4] = 1;
                            currentDayTime[5]++;
                        }
                        break;
                }
                //check end of year
                if(currentDayTime[5]==13){
                    currentDayTime[5] = 1;
                }

            }
        }
    }
}

int getHours(){
    return currentDayTime[0];
}

int getMins(){
    return currentDayTime[1];
}

int getSeconds(){
    return currentDayTime[2];
}

int getDOTW(){
    return currentDayTime[3];
}

int getDate(){
    return currentDayTime[4];
}

int getMonth(){
    return currentDayTime[5];
}


void time () {
    Current_State = STATE_MINS;
    int NEW_STATE = 1;

    //do something
    while(1){
        if(getSTARTSTOP()==1){
            NEW_STATE = 1;
        switch (Current_State) {
            case STATE_MINS:
                Current_State = STATE_HOURS;
                break;
            case STATE_HOURS:
                Current_State = STATE_DOTW;
                break;
            case STATE_DOTW:
                Current_State = STATE_MONTH;
                break;
            case STATE_DATE:
                Current_State = STATE_MINS;
                break;
            case STATE_MONTH:
                Current_State = STATE_DATE;
                break;
            default:
                break;
        }
            clearSTARTSTOP();
        }  
        __disable_interrupt();
        switch (Current_State) {

        case STATE_MINS:
            if(NEW_STATE == 1 || process_switched == 1){
                LCD_ClearNums();
                setLCDMins();
                setLCDHours();
                setLCDDotW();
                LCD_setColon();
                LCD_setBlink(3);
                LCD_setBlink(4);
                NEW_STATE = 0;
                clearTimeSwitch();
            }
            if(check_SW3()==1){
                currentDayTime[1]++;
                if(currentDayTime[1]==60){
                    currentDayTime[1] = 0;
                }
                setLCDMins();
                LCD_setBlink(3);
                LCD_setBlink(4);
            }

            break;

        case STATE_HOURS:
            if(NEW_STATE == 1 || process_switched == 1){
                LCD_ClearNums();
                setLCDMins();
                setLCDHours();
                setLCDDotW();
                LCD_setColon();
                LCD_setBlink(1);
                LCD_setBlink(2);
                NEW_STATE = 0;
                clearTimeSwitch();
            }
            if(check_SW3()==1){
                currentDayTime[0]++;
                if(currentDayTime[0]==24){
                    currentDayTime[0] = 0;
                }
                setLCDHours();
                LCD_setBlink(1);
                LCD_setBlink(2);
            }
            break;

        case STATE_DOTW:
            if(NEW_STATE == 1 || process_switched == 1){
                LCD_ClearNums();
                setLCDMins();
                setLCDHours();
                setLCDDotW();
                LCD_setColon();
                LCD_setBlink(5);
                LCD_setBlink(6);
                NEW_STATE = 0;
                clearTimeSwitch();
            }
            if(check_SW3()==1){
                currentDayTime[3]++;
                if(currentDayTime[3]==7){
                    currentDayTime[3] = 0;
                }
                setLCDDotW();
                LCD_setBlink(5);
                LCD_setBlink(6);
            }
            break;

        case STATE_MONTH:
            if(NEW_STATE == 1 || process_switched == 1){
                LCD_ClearNums();
                setLCDMonth();
                setLCDDate();
                setLCDDotW();
                LCD_setDecimals();
                LCD_setBlink(3);
                LCD_setBlink(4);
                NEW_STATE = 0;
                clearTimeSwitch();
            }
            if(check_SW3()==1){
                currentDayTime[5]++;
                if(currentDayTime[5]==13){
                    currentDayTime[5] = 1;
                }
                setLCDMonth();
                LCD_setBlink(3);
                LCD_setBlink(4);
            }
            break;

        case STATE_DATE:
            if(NEW_STATE == 1 || process_switched == 1){
                LCD_ClearNums();
                setLCDMonth();
                setLCDDate();
                setLCDDotW();
                LCD_setDecimals();
                LCD_setBlink(1);
                LCD_setBlink(2);
                NEW_STATE = 0;
                clearTimeSwitch();
            }
            if(check_SW3()==1){
                currentDayTime[4]++;
                switch(currentDayTime[5]){
                    case 1:
                    case 3:
                    case 5:
                    case 7:
                    case 8:
                    case 10:
                    case 12:
                        if(currentDayTime[4]>31){
                            currentDayTime[4] = 1;
                        }
                        break;
                    case 2:
                        if(currentDayTime[4]>28){
                            currentDayTime[4] = 1;
                        }
                        break;
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        if(currentDayTime[4]>30){
                            currentDayTime[4] = 1;
                        }
                        break;
                }
                setLCDDate();
                LCD_setBlink(1);
                LCD_setBlink(2);
            }
            break;

        default:
            break;
        }
        __enable_interrupt();    
    }
}

