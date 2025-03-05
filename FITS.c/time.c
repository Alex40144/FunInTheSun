//Hour, Min, Weekday, Date(Day), Monrth
//Weekdays0 = Monday, 6 = Sunday
int currentDayTime[] = {1,11,0,0,0};
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


void time () {
    //do something
    while(1){
        int i =0;
            for(i;i<30000;i++){
        if(i==29999){
                setLCDHours();
                setLCDMins();
                LCD_setColon();
                LCD_WriteSingle(' ', 6);
                i = 0;
        }
    }
    }
}

