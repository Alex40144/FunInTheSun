void LCD_setAlarm();
void LCD_clearAlarm();
void LCD_setStopWatch();
void LCD_clearStopWatch();
void LCD_setChrono();
void LCD_clearChrono();
void LCD_setColon();
void LCD_clearColon();
void LCD_setDecimalOne();
void LCD_clearDecimalOne();
void LCD_setDecimalTwo();
void LCD_clearDecimalTwo();
void LCD_setDecimals();
void LCD_clearDecimals();
void LCD_setBlink(int position);
void LCD_clearBlink(int position);
int LCD_INIT( void );
void LCD_WriteAll(char text1, char text2, char text3, char text4, char text5 );
void LCD_WriteSingle(char text, int position);
void LCD_ClearNums();
