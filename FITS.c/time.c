//Hour, Min, Weekday, Date(Day), Monrth
//Weekdays0 = Monday, 6 = Sunday
#include <stdio.h>

int currentDayTime[] = {1,11,3,15,9};

enum States{STATE_MINS,STATE_HOURS,STATE_DOTW,STATE_DATE,STATE_MONTH}Current_State;


int setLCDHours(){
    LCD_ClearNums();
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
    switch(currentDayTime[2]){
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
    int Date = currentDayTime[3];
    int dateDigitOne = Date / 10;
    int dateDigitTwo = Date % 10;
    LCD_WriteSingle(dateDigitOne,1);
    LCD_WriteSingle(dateDigitTwo,2);
}

int setLCDMonth(){
    int Month = currentDayTime[4];
    int monthDigitOne = Month / 10;
    int monthDigitTwo = Month % 10;
    LCD_WriteSingle(monthDigitOne,3);
    LCD_WriteSingle(monthDigitTwo,4);
}


void time () {
    Current_State = STATE_MINS;
    //do something
    while(1){
        switch (Current_State) {
        case STATE_MINS:
            break;
        case STATE_HOURS:
            break;
        case STATE_DOTW:
            break;
        case STATE_DATE:
            break;
        case STATE_MONTH:
            break;
        default:
            break;
        }

        int i =0;
            for(i;i<30000;i++){
                int var = getSTARTSTOP();
                //test();
        if(i==29999){
            if(var == 1){
                setLCDDate();
                setLCDMonth();
                LCD_setDecimals();
                clearSTARTSTOP();
                i = 0;
            }else{
                setLCDHours();
                setLCDMins();
                setLCDDotW();
                LCD_setColon();
            }
        }
    }
    }
}

