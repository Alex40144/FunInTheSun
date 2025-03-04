int setLCDHours();
int setLCDMins();
int 

void time () {
    //do something
    while(1){
        int i =0;
            for(i;i<30000;i++){
        if(i==29999){
                LCD_WriteAll('T', 'I', 'M', 'E', ' ');
                LCD_WriteSingle(' ', 6);
                i = 0;
        }
    }
    }
}

