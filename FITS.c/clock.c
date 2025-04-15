#include "LCD.h"
#include "time.h"
#include "clock.h"
#include "alarm.h"
#include <msp430.h>

#define MODE_BUTTON    BIT2  // P1.2 - Mode Button
#define START_STOP     BIT6  // P1.3 - Start/Stop Button
#define LAP_RESET      BIT4  // P1.4 - Lap/Reset Button

// Global Variables
static int alarmON = 0;
static int chimeON = 0;

// Function Prototypes
void displayClockTime();
void displayClockDate();
void displayClockAlarm();
void toggleAlarm();
void toggleChime();
void initButtons();
void delay_ms(unsigned int ms);
void setClockMode();
void clockMode();

// Initialize buttons as inputs
void initButtons() {
    P1REN |= (MODE_BUTTON | START_STOP | LAP_RESET);
    P1OUT |= (MODE_BUTTON | START_STOP | LAP_RESET);
}

// Delay function (Replaces __delay_cycles)
void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000);  // Adjust based on clock speed
    }
}

// Display Functions - Now Uses `getHours()`, `getMins()`, etc.
void displayClockTime() {
    int hours = getHours();  // Get real-time hours
    int mins = getMins();    // Get real-time minutes

    int hourDigitOne = hours / 10;
    int hourDigitTwo = hours % 10;
    int minDigitOne = mins / 10;
    int minDigitTwo = mins % 10;

    LCD_WriteSingle(hourDigitOne, 1);
    LCD_WriteSingle(hourDigitTwo, 2);
    LCD_WriteSingle(minDigitOne, 3);
    LCD_WriteSingle(minDigitTwo, 4);
    LCD_setColon();
    int day = getDOTW();
    switch (day) {
        case 0:
        LCD_WriteSingle('M', 5);
        LCD_WriteSingle('o', 6);
    break;
    case 1:
        LCD_WriteSingle('T', 5);
        LCD_WriteSingle('u', 6);
    break;
    case 2:
        LCD_WriteSingle('W', 5);
        LCD_WriteSingle('e', 6);
    break;
    case 3:
        LCD_WriteSingle('T', 5);
        LCD_WriteSingle('h', 6);
    break;
    case 4:
        LCD_WriteSingle('F', 5);
        LCD_WriteSingle('r', 6);
    break;
    case 5:
        LCD_WriteSingle('S', 5);
        LCD_WriteSingle('a', 6);
    break;
    case 6:
        LCD_WriteSingle('S', 5);
        LCD_WriteSingle('u', 6);
    break;
    }
}

void displayClockDate() {
    int date = getDate();  // Get real-time date
    int month = getMonth(); // Get real-time month

    int dateDigitOne = date / 10;
    int dateDigitTwo = date % 10;
    int monthDigitOne = month / 10;
    int monthDigitTwo = month % 10;

    LCD_WriteSingle(dateDigitOne, 1);
    LCD_WriteSingle(dateDigitTwo, 2);
    LCD_WriteSingle(monthDigitOne, 3);
    LCD_WriteSingle(monthDigitTwo, 4);
    LCD_setDecimals();
}

void displayClockAlarm() {
    int alarmHour = Alarm_getAlarmHours();
    int alarmMin = Alarm_getAlarmMins();

    int hourDigitOne = alarmHour / 10;
    int hourDigitTwo = alarmHour % 10;
    int minDigitOne = alarmMin / 10;
    int minDigitTwo = alarmMin % 10;

    LCD_WriteSingle(hourDigitOne, 1);
    LCD_WriteSingle(hourDigitTwo, 2);
    LCD_WriteSingle(minDigitOne, 3);
    LCD_WriteSingle(minDigitTwo, 4);
    LCD_setColon();
    
    if (alarmON) {
        LCD_setAlarm();
    } else {
        LCD_clearAlarm();
    }

    if (chimeON) {
        LCD_setTxRx();
    } else {
        LCD_clearTxRx();
    }
}

// Toggle Functions
void toggleAlarm() {
    alarmON = !alarmON;
    if (alarmON) {
        LCD_setAlarm();
    } else {
        LCD_clearAlarm();
    }
}

void toggleChime() {
    chimeON = !chimeON;
    if (chimeON) {
        LCD_setDecimals();
    } else {
        LCD_clearDecimals();
    }
}

// Clock Mode State Machine
void clock() {
    initButtons();
    displayClockTime();  // Show time on startup

    while (1) {

        

        if ((!(P1IN & LAP_RESET)) && (!(P2IN & START_STOP))) {  // Toggle Alarm
            toggleAlarm();
        } else if (!(P1IN & LAP_RESET) && !(P1IN & MODE_BUTTON)) {  // Toggle Chime
            toggleChime();
        } else if (!(P2IN & START_STOP)) {  // Show Month/Date
            displayClockDate();
        } else if (!(P1IN & LAP_RESET)) {  // Show Alarm Mode
            displayClockAlarm();
        } else {
            displayClockTime();
        }
        delay_ms(100);
    }
}
