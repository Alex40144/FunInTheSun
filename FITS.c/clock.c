void clock () {
    //do something
        int i = 0;
    for(i;i<30000;i++){
        if(i==29999){
                LCD_WriteAll('C', 'L', 'K', ' ', ' ');
                LCD_WriteSingle(' ', 6);
                i = 0;
        }
    }
    
}
