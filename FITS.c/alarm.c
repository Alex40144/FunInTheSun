#include "intrinsics.h"
#include "alarm.h"
#include "time.h"

/* -----------------------------------------------------------------------------
  Static memory allocation for the time of the alarm of the device
    [0] = Hours (0-23)
    [1] = Minutes (0-59)
------------------------------------------------------------------------------*/
int currentAlarmHoursMins[] = {12,0};

/*F ----------------------------------------------------------------------------
  NAME :      Alarm_getAlarmHours() and Alarm_GetAlarmMins()

  DESCRIPTION :
                Returns the current time value for the alarm to modules outside of the scope of alarm.c.

  INPUTS :      void

  RETURNS :     int

  PROCESS :
                Returns the relevant value.

*F ---------------------------------------------------------------------------*/
int Alarm_getAlarmHours() // global function to retrieve the alarm hours setting
{
    return currentAlarmHoursMins[0];
}

int Alarm_getAlarmMins() // global function to retrieve the alarm minutes setting
{
    return currentAlarmHoursMins[1];
}

/*F ----------------------------------------------------------------------------
  NAME :      setLCDAlarmHours() and setLCDAlarmMins()

  DESCRIPTION :
                Calculates and writes to the LCD: the latest values for Hours/Mins setting of the alarm.

  INPUTS :      void

  RETURNS :     void

  PROCESS :
    Gets the values for Hours/Mins and applies a '/' operator for the most-significant digit and a '%' for the least-signifcant digit.
    Calls LCD_WriteSingle(value, position) to write the calculated hours/mins digits to the UI.

*F ---------------------------------------------------------------------------*/
void setLCDAlarmHours(){ // writes the latest values for hours to the alarm UI
    int hours = currentAlarmHoursMins[0];
    int hourDigitOne = hours / 10;
    int hourDigitTwo = hours % 10;
    LCD_WriteSingle(hourDigitOne,1);
    LCD_WriteSingle(hourDigitTwo,2);
}

void setLCDAlarmMins(){  // writes the latest values for minutes to the alarm UI
    int mins = currentAlarmHoursMins[1];
    int minDigitOne = mins / 10;
    int minDigitTwo = mins % 10;
    LCD_WriteSingle(minDigitOne,3);
    LCD_WriteSingle(minDigitTwo,4);
}

/*F ----------------------------------------------------------------------------
  NAME :      setLCDAlarmHoursNull() and setLCDAlarmMinsNull()

  DESCRIPTION :
                Replaces the hours/mins digits of the alarm UI with blanks

  INPUTS :      void

  RETURNS :     void

  PROCESS :
    Calls LCD_WriteSingle(value, position) to wchoose which position to set a blank digit to the UI.

*F ---------------------------------------------------------------------------*/
void setLCDAlarmHoursNull(){ // replaces the hours digits of the alarm UI with blanks
    LCD_WriteSingle(' ',1);
    LCD_WriteSingle(' ',2);
}

void setLCDAlarmMinsNull(){  // replaces the minutes digits of the alarm UI with blanks
    LCD_WriteSingle(' ',3);
    LCD_WriteSingle(' ',4);
}

void alarm () {

/* -----------------------------------------------------------------------------
  Static states which the alarm module can be focusing on based on user inputs.
  States mean that their corresponding data is flashing can be modified by the user
------------------------------------------------------------------------------*/
typedef enum {
    MINUTES,
    HOURS
} AlarmProcessStates;
AlarmProcessStates currentState = MINUTES; // defaults to minutes at startup


while(1){ // continuous loop for the main control code of the alarm module
            switch (currentState) { // decide which state to focus

            case MINUTES: // allow setting of and flash the minutes section on LCD
                __delay_cycles(100000); // delay for user to view
                // set the values for the alarm
                setLCDAlarmHours();
                setLCDAlarmMins();
                // set the UI required pieces: colon, dot, and alarm symbol
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000); // delay for user to view
                // set the values for the alarm
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDAlarmMinsNull(); // replace the mins with blank digits to show it flashing to the user
                // set the UI required pieces: colon, dot, and alarm symbol
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

                // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to hours state
                __delay_cycles(50000);
                if (!(P1IN & BIT4)) { // check if lap/reset button pressed
                    __delay_cycles(150000); // wait
                    if (!(P1IN & BIT4)) { // Check again if switch is still pressed for software debouncing
                        currentState = HOURS;
                    } 
                    }
                
                // check if START/STOP is pressed to increment alarm time
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000); // wait
                    while (!(P2IN & BIT6)) { // Check again if switch is still pressed for software debouncing
                        currentAlarmHoursMins[1] = currentAlarmHoursMins[1] + 1;  // increment the minutes value of the alarm
                        if (currentAlarmHoursMins[1] == 60) // check if passed 60 minute clock limit
                        {
                            currentAlarmHoursMins[1] = 0; // reset to minimum
                        }
                        setLCDAlarmMins();
                        __delay_cycles(350000); // add delay to prevent incrementing the value too quickly
                    } // Check again if switch is still pressed}
                }
            break;


            case HOURS: // allow setting of and flashing of the hours digits of the display
                __delay_cycles(100000); // delay for user to view
                // set the values for the alarm
                setLCDAlarmHours();
                setLCDAlarmMins();
                // set the UI required pieces: colon, dot, and alarm symbol
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

                __delay_cycles(150000); // delay for user to view
                // set the values for the alarm
                setLCDAlarmHours();
                setLCDAlarmMins();
                setLCDAlarmHoursNull(); // replace the hours with blank digits to show it flashing to the user
                // set the UI required pieces: colon, dot, and alarm symbol
                setLCDDotW();
                LCD_setAlarm();
                LCD_setColon();

            // if start/stop button pressed, increment digits
                //if lap/reset button pressed move to minutes state
                __delay_cycles(50000); // delay for user to view
                if (!(P1IN & BIT4)) { // check if lap/reset button pressed
                    __delay_cycles(150000); // wait
                    if (!(P1IN & BIT4)) { // Check again if switch is still pressed for software debouncing
                        currentState = MINUTES;
                        __delay_cycles(100000);
                    } // Check again if switch is still pressed}
                    }

                // check if START/STOP is pressed to increment alarm time
                if (!(P2IN & BIT6)) {
                    __delay_cycles(150000);
                    while (!(P2IN & BIT6)) { // Check again if switch is still pressed for software debouncing
                        currentAlarmHoursMins[0] = currentAlarmHoursMins[0] + 1; // increment the hours value of the alarm
                        if (currentAlarmHoursMins[0] == 24) // check if passed 24 hour clock limit
                        {
                            currentAlarmHoursMins[0] = 0; // reset to minimum
                        }
                        setLCDAlarmHours();
                        __delay_cycles(350000); // add delay to prevent incrementing the value too quickly
                    }
                }
            break;
        }

}
    }
