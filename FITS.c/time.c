/* -----------------------------------------------------------------------------
    Include libraries
    [intrinsics.h] for delay functionality and interrupt enabling and disabling
    [stdio.h] for basic operations
    [msp430.h] for board specific registers
------------------------------------------------------------------------------*/
#include "intrinsics.h"
#include <stdio.h>
#include <msp430.h>

/* -----------------------------------------------------------------------------
    This integer is used to detect if the time module has been enabled by the process switcher
------------------------------------------------------------------------------*/
int process_switched = 0;
/* -----------------------------------------------------------------------------
  Static memory allocation for the time/ date storage of the device
    [0] = Hours
    [1] = Minutes
    [2] = Seconds
    [3] = Day of the Week
    [4] = Date
    [5] = Month
------------------------------------------------------------------------------*/
int currentDayTime[] = {1,11,0 ,3,15,9};
/* -----------------------------------------------------------------------------
  Static states which the time module can be.
  States mean that their corresponding data is flashing can be modified by the user
------------------------------------------------------------------------------*/
enum States{STATE_MINS,STATE_HOURS,STATE_DOTW,STATE_DATE,STATE_MONTH}Current_State;

/*F ----------------------------------------------------------------------------
  NAME :      setTimeSwitch() and clearTimeSwitch()

  DESCRIPTION :
                These are used to detect when the time module has been switched into or away from
                They allow the main time code function to display information when called
                It operates with a boolean value:
                0 = Time mode is active and the display has been printed
                1 = Time module needs to be displayed again as the main process has changed

  INPUTS :      void

  RETURNS :     void

  PROCESS :
    set()     [1]   set process switch high
    clear()   [1]   set process switch low

*F ---------------------------------------------------------------------------*/
void setTimeSwitch(){
    process_switched = 1;
}

void clearTimeSwitch(){
    process_switched = 0;
}

/*F ----------------------------------------------------------------------------
  NAME :      check_SW3()

  DESCRIPTION :
                This is used to check the status of the lap/reset switch (SW3)
                If the input is low then the switch is depressed
                If the input is high then the switch is released

  INPUTS :      void

  RETURNS :     
                boolean:
                    [0] Switch released
                    [1] Switch depressed

  PROCESS :
                [1]   Read the value of the switches port (Port 1 pin 4)
                [2]   Is the switch depressed (low)? 
                        Yes see [3] No see [6]
                [3]   Wait for 100ms
                [4]   Is the switch depressed (low)? 
                        Yes see [5] No see [6]
                [5]   Return the value 1 and end
                [6]   Return the value 0 and end

*F ---------------------------------------------------------------------------*/
int check_SW3(){
    if (!(P1IN & BIT4)) {
        __delay_cycles(400000);
        if (!(P1IN & BIT4)) {
            return 1;
        } // Check again if switch is still pressed}
    }
    return 0;
}


/*F ----------------------------------------------------------------------------
  NAME :      setLCDHours()

  DESCRIPTION :
                Writes the current stored hour value to the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Read hours value from memory
                [2]     Split the digits individually
                [3]     Write each digit to the LCD positions 1 and 2

*F ---------------------------------------------------------------------------*/
int setLCDHours(){
    int hours = currentDayTime[0];
    int hourDigitOne = hours / 10;
    int hourDigitTwo = hours % 10;
    LCD_WriteSingle(hourDigitOne,1);
    LCD_WriteSingle(hourDigitTwo,2);
}

/*F ----------------------------------------------------------------------------
  NAME :      setLCDMins()

  DESCRIPTION :
                Writes the current stored minutes value to the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Read minutes value from memory
                [2]     Split the digits individually
                [3]     Write each digit to the LCD positions 3 and 4

*F ---------------------------------------------------------------------------*/
int setLCDMins(){
    int mins = currentDayTime[1];
    int minDigitOne = mins / 10;
    int minDigitTwo = mins % 10;
    LCD_WriteSingle(minDigitOne,3);
    LCD_WriteSingle(minDigitTwo,4);
}

/*F ----------------------------------------------------------------------------
  NAME :      setLCDDotW()

  DESCRIPTION :
                Writes the current stored day of the week to the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Decide which day of the week it is from the memory
                [2]     Depending on the day write the corresponding letter codes to the LCD in positions 5 and 6
                [3]     If the day cannot be decoded write XX to the screen

*F ---------------------------------------------------------------------------*/
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

/*F ----------------------------------------------------------------------------
  NAME :      setLCDDate()

  DESCRIPTION :
                Writes the current stored date to the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Read date value from memory
                [2]     Split the digits individually
                [3]     Write each digit to the LCD positions 1 and 2

*F ---------------------------------------------------------------------------*/
int setLCDDate(){
    int Date = currentDayTime[4];
    int dateDigitOne = Date / 10;
    int dateDigitTwo = Date % 10;
    LCD_WriteSingle(dateDigitOne,1);
    LCD_WriteSingle(dateDigitTwo,2);
}

/*F ----------------------------------------------------------------------------
  NAME :      setLCDMonth()

  DESCRIPTION :
                Writes the current stored month to the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Read month value from memory
                [2]     Split the digits individually
                [3]     Write each digit to the LCD positions 3 and 4

*F ---------------------------------------------------------------------------*/
int setLCDMonth(){
    int Month = currentDayTime[5];
    int monthDigitOne = Month / 10;
    int monthDigitTwo = Month % 10;
    LCD_WriteSingle(monthDigitOne,3);
    LCD_WriteSingle(monthDigitTwo,4);
}

/*F ----------------------------------------------------------------------------
  NAME :      clkIncrement()
  DESCRIPTION :
                Updates the time and date stored in memory once per second
                Triggered from the interrupt found in main on timer A1

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Increment the seconds value
                [2]     Check seconds overflow and reset it to 0 and increment minutes
                [3]     If overflow occured check minutes haven't overflowed
                [4]     If overflow occured increment the hours and reset minutes to 0
                [5]     If hours have overflowed the increment date and day of the week and check for overflows
                [6]     If date overflows based on the number of day in the month reset to 1 and increment months
                [7]     If months overflows reset to 1

*F ---------------------------------------------------------------------------*/
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

/*F ----------------------------------------------------------------------------
  NAME :      getHours()

  DESCRIPTION :
                Called by external functions to return the hours information stored in memory

  INPUTS :      void

  RETURNS :     integer: hours value 0-23

  PROCESS :
                [1]     Read hours value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getHours(){
    return currentDayTime[0];
}

/*F ----------------------------------------------------------------------------
  NAME :      getMins()
  DESCRIPTION :
                Called by external functions to return the minutes information stored in memory

  INPUTS :      void

  RETURNS :     integer: minutes value 0-59

  PROCESS :
                [1]     Read minutes value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getMins(){
    return currentDayTime[1];
}

/*F ----------------------------------------------------------------------------
  NAME :      getSeconds()
  DESCRIPTION :
                Called by external functions to return the seconds information stored in memory

  INPUTS :      void

  RETURNS :     integer: seconds value 0-59

  PROCESS :
                [1]     Read seconds value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getSeconds(){
    return currentDayTime[2];
}

/*F ----------------------------------------------------------------------------
  NAME :      getDOTW()
  DESCRIPTION :
                Called by external functions to return the day of the week information stored in memory

  INPUTS :      void

  RETURNS :     integer: day of the week value 0-6
                0 = Monday
                1 = Tuesday
                ...
                6 = Sunday

  PROCESS :
                [1]     Read day of the week value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getDOTW(){
    return currentDayTime[3];
}

/*F ----------------------------------------------------------------------------
  NAME :      getSeconds()
  DESCRIPTION :
                Called by external functions to return the date information stored in memory

  INPUTS :      void

  RETURNS :     integer: date value 1-31 [This is the max range, Feb is 1-28 and other months are 1-30]

  PROCESS :
                [1]     Read date value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getDate(){
    return currentDayTime[4];
}

/*F ----------------------------------------------------------------------------
  NAME :      getMonth()
  DESCRIPTION :
                Called by external functions to return the month information stored in memory

  INPUTS :      void

  RETURNS :     integer: month value 1-12

  PROCESS :
                [1]     Read month value from memory
                [2]     Return it to external function

*F ---------------------------------------------------------------------------*/
int getMonth(){
    return currentDayTime[5];
}

/*F ----------------------------------------------------------------------------
  NAME :      time()
  DESCRIPTION :
                Main function for the time module
                Updates the display accordinf to user actions
                Used to store the updated information to memory for other modules to use
                Enables the blinking of updating numbers
                Contains a finite state machine to loop through the updating digits in the white paper's order
                Only updates the display if a user makes an action

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the initial state and enable LCD printing without a users intervention for one cycle

                [2]     Loop
                    [2.1.1]     Check if the user wants to change which number they are updating e.g go from minutes to seconds
                    [2.1.2]     Update the state to the next state based on the current state
                    
                    [2.2.1]     Disables interrupts to prevent memory access clashing
                    [2.2.2]     Identify the state the user is in
                    [2.2.3]     Based on the state information display the screen which the user needs to see (First time only to prevent screen glitches)
                    [2.2.4]     Set the current updating number to a blinking state
                    [2.2.4]     Check if the user is pressing the lap/reset button to increment the blinking number
                    [2.2.5]     Check the new number is not out of range and reset it if it is
                    [2.2.6]     Print out the updated number and set it blinking again
                    [2.2.7]     Enable interrupts

*F ---------------------------------------------------------------------------*/
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
