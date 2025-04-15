#include "LCD.h"
#include "time.h"
#include "clock.h"
#include "alarm.h"
#include <msp430.h>

#define MODE_BUTTON    BIT2  // P1.2 - Mode Button
#define START_STOP     BIT3  // P1.3 - Start/Stop Button
#define LAP_RESET      BIT4  // P1.4 - Lap/Reset Button

// State Machine States
enum ClockState { CLOCK_MODE, MONTH_DATE_DISPLAY, ALARM_DISPLAY, TIME_SETTING_MODE };
enum ClockState currentState = CLOCK_MODE;

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
    P1DIR &= ~(MODE_BUTTON | START_STOP | LAP_RESET);
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
    
    if (alarmON) {
        LCD_setAlarm();
    } else {
        LCD_clearAlarm();
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
    currentState = CLOCK_MODE;  // Default state
    displayClockTime();  // Show time on startup

    while (1) {
        if (!(P1IN & MODE_BUTTON)) {  // Enter Time Setting Mode
            delay_ms(50);
            // while (!(P1IN & MODE_BUTTON));
            // delay_ms(50);
            // setClockMode();
        }

        if (!(P1IN & START_STOP)) {  // Show Month/Date
            delay_ms(50);
            while (!(P1IN & START_STOP));
            delay_ms(50);
            currentState = MONTH_DATE_DISPLAY;
            displayClockDate();
        }

        if (!(P1IN & LAP_RESET)) {  // Show Alarm Mode
            delay_ms(50);
            while (!(P1IN & LAP_RESET));
            delay_ms(50);
            currentState = ALARM_DISPLAY;
            displayClockAlarm();
        }

        if (!(P1IN & LAP_RESET) && !(P1IN & START_STOP)) {  // Toggle Alarm
            delay_ms(50);
            toggleAlarm();
        }

        if (!(P1IN & LAP_RESET) && !(P1IN & MODE_BUTTON)) {  // Toggle Chime
            delay_ms(50);
            toggleChime();
        }

        if (currentState == CLOCK_MODE) {  
            displayClockTime();  // Ensure time is displayed when idle
        }
    }
}
