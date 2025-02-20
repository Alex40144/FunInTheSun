#include <msp430.h>
#include "LCDHEADER.h"

int main( void )
{
    WDTCTL = WDTPW | WDTHOLD;                                  // Stop watchdog timer
    LCD_INIT();
    LCD_TEXT('1','2','3','4','5','6');
    void setAlarm();
    void clearAlarm();
}