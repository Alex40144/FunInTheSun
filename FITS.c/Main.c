#include "LCD.h"

int main(void){
    LCD_INIT();
    setAlarm();
    clearAlarm();
    LCD_WriteAll('1','2','D','Z','A','9');
    LCD_WriteSingle('F', 3);
}
