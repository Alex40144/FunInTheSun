void alarm () {
    //do something
while(1){
        int i = 0;
    for(i;i<30000;i++){
        if(i==29999){
                LCD_WriteAll('A', 'L', 'A', 'R', 'M');
                LCD_WriteSingle(' ', 6);
                i = 0;
        }
    }
}
    }
