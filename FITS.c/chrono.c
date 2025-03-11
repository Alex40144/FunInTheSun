void chrono () {
    //do something
    while(1){
        int i = 0;
    for(i;i<30000;i++){
        if(i==29999){
                LCD_WriteAll('C', 'H', 'R', 'O', 'N');
                LCD_WriteSingle('O', 6);
                i = 0;
        }
    }


    }
}