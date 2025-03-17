
/* -----------------------------------------------------------------------------
    Include libraries
    [msp430.h] for board specific registers
------------------------------------------------------------------------------*/
#include <msp430.h>

/* -----------------------------------------------------------------------------
    Function prototypes
------------------------------------------------------------------------------*/
void showChar(char c, int position);
void showCharB(char c, int position);

/* -----------------------------------------------------------------------------
    The LCD memory areas for each digit are non sequential
    Positions below correspond from left to right of the LCD
------------------------------------------------------------------------------*/
#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18
#define pos7 1                                                // Digit A6 - L18

/* -----------------------------------------------------------------------------
    Lookup table for the positions for use with array data
------------------------------------------------------------------------------*/
int posLookUp[] = {4,6,8,10,2,18,1};

/* -----------------------------------------------------------------------------
    Definitions of the LCD memory to make writing to them simpler
    The memory consists of two parts which both need to be written to to make a static number
    LCDMEMW is the first half
    LCDBMEMW is the second half
------------------------------------------------------------------------------*/
#define LCDMEMW ((int*)LCDMEM)
#define LCDBMEMW ((int*)LCDBMEM)

/* -----------------------------------------------------------------------------
    LCD memory for each digit to be displayed
------------------------------------------------------------------------------*/
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

/* -----------------------------------------------------------------------------
    LCD memory for upper case letter to be displayed
------------------------------------------------------------------------------*/
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

/* -----------------------------------------------------------------------------
    LCD memory for lower case letter to be displayed
------------------------------------------------------------------------------*/
const char alphabetSmall[26][2] =
{
    {0xFB, 0x00},  /* "a" */
    {0x0F, 0x22},  /* "b" */
    {0x1B, 0x00},  /* "c" */
    {0x3D, 0x00},  /* "d" */
    {0x9F, 0x00},  /* "e" */
    {0x87, 0x00},  /* "f" */
    {0xBD, 0x00},  /* "g" */
    {0x0E, 0x02},  /* "h" */
    {0x10, 0x00},  /* "i" */
    {0x78, 0x00},  /* "j" */
    {0x0E, 0x22},  /* "k" */
    {0x1C, 0x00},  /* "l" */
    {0x6C, 0xA0},  /* "m" */
    {0x0C, 0x02},  /* "n" */
    {0x1F, 0x00},  /* "o" */
    {0x8F, 0x00},  /* "p" */
    {0xFC, 0x02},  /* "q" */
    {0x0C, 0x02},  /* "r" */
    {0xB7, 0x00},  /* "s" */
    {0x80, 0x50},  /* "t" */
    {0x3C, 0x00},  /* "u" */
    {0x0C, 0x28},  /* "v" */
    {0x6C, 0x0A},  /* "w" */
    {0x00, 0xAA},  /* "x" */
    {0x00, 0xB0},  /* "y" */
    {0x90, 0x28}   /* "z" */
};

/*F ----------------------------------------------------------------------------
  NAME :      showChar()

  DESCRIPTION :
                Writes a provided character to a specific position on the LCD screen

  INPUTS :      
                char c : This is the character which is to be displayed on the LCD
                int position : This is the position (1-6) where the character needs to be displayed

  RETURNS :     void

  PROCESS :
                [1]     Identify which type of character it is (Space, Upper case, Lower case, Digit, Erroneous)
                [2]     Identify the correct character memory map
                [3]     Read memory map information
                [4]     Write the memory information to the first half of the memory
                [5]     Write the memory information to the second half of the memory
*F ---------------------------------------------------------------------------*/
 void showChar(char c, int position)
 {
     if (c == ' ')
     {
         // Display space
         LCDMEMW[position/2] = 0;
         LCDBMEMW[position/2] = 0;
     }
     else if (c >= '0' && c <= '9')
     {
         // Display digit
         LCDMEMW[position/2] = digit[c-48][0] | (digit[c-48][1] << 8);
         LCDBMEMW[position/2] = digit[c-48][0] | (digit[c-48][1] << 8);
     }
    else if (c >= 0 && c <= 9)
     {
         // Display digit
         LCDMEMW[position/2] = digit[c][0] | (digit[c][1] << 8);
         LCDBMEMW[position/2] = digit[c][0] | (digit[c][1] << 8);
     }
     else if (c >= 'A' && c <= 'Z')
     {
         // Display alphabet
         LCDMEMW[position/2] = alphabetBig[c-65][0] | (alphabetBig[c-65][1] << 8);
         LCDBMEMW[position/2] = alphabetBig[c-65][0] | (alphabetBig[c-65][1] << 8);
     }
     else if (c >= 'a' && c <= 'z')
     {
         // Display alphabet
         LCDMEMW[position/2] = alphabetSmall[c-97][0] | (alphabetSmall[c-97][1] << 8);
         LCDBMEMW[position/2] = alphabetSmall[c-97][0] | (alphabetSmall[c-97][1] << 8);
     }
     else
     {
         // Turn all segments on if character is not a space, digit, or uppercase letter
         LCDMEMW[position/2] = 0xFFFF;
         LCDBMEMW[position/2] = 0xFFFF;
     }
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setAlarm()

  DESCRIPTION :
                Enables the alarm icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the alarm icon so it enables
*F ---------------------------------------------------------------------------*/
 void LCD_setAlarm(){
    int position = 12;
    LCDMEMW[position/2] |= 0x2;
    LCDBMEMW[position/2] |= 0x2;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearAlarm()

  DESCRIPTION :
                Disables the alarm icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the alarm icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearAlarm(){
    int position = 12;
    LCDMEMW[position/2] &= ~0x2;
    LCDBMEMW[position/2] &= ~0x2;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setStopWatch()

  DESCRIPTION :
                Enables the stopwatch icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the stopwatch icon so it enables
*F ---------------------------------------------------------------------------*/
  void LCD_setStopWatch(){
    int position = 12;
    LCDMEMW[position/2] |= 0x8;
    LCDBMEMW[position/2] |= 0x8;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearStopWatch()

  DESCRIPTION :
                Disables the stopwatch icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the stopwatch icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearStopWatch(){
    int position = 12;
    LCDMEMW[position/2] &= ~0x08;
    LCDBMEMW[position/2] &= ~0x08;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setChrono()

  DESCRIPTION :
                Enables the chrono icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the chrono spike icon so it enables
                [2]     Set the LCD to display a "1" in the location of the rx/tx icon so it enables
*F ---------------------------------------------------------------------------*/
  void LCD_setChrono(){
    LCDMEMW[4] |= 0x0400;//The spike symbol
    LCDBMEMW[4] |= 0x0400;
    LCDMEMW[9] |= 0x0500;//The TX RX symbol
    LCDBMEMW[9] |= 0x0500;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearChrono()

  DESCRIPTION :
                Disables the chrono icon on the LCD

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the chrono spike icon so it disables
                [2]     Set the LCD to display a "0" in the location of the rx/tx icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearChrono(){
    LCDMEMW[4] &= ~0x0400;
    LCDBMEMW[4] &= ~0x0400;
    LCDMEMW[9] &= ~0x0500;
    LCDBMEMW[9] &= ~0x0500;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setColon()

  DESCRIPTION :
                Enables the colon icon on the LCD between characters 2 and 3

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the colon icon so it enables
*F ---------------------------------------------------------------------------*/
   void LCD_setColon(){
    LCDMEMW[3] |= 0x0400;
    LCDBMEMW[3] |= 0x0400;

 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearColon()

  DESCRIPTION :
                Disables the colon icon on the LCD between characters 2 and 3

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the colon icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearColon(){
    LCDMEMW[3] &= ~0x0400;
    LCDBMEMW[3] &= ~0x0400;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setDecimalOne()

  DESCRIPTION :
                Enables the decimal icon on the LCD between characters 2 and 3

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the decimal icon so it enables
*F ---------------------------------------------------------------------------*/
   void LCD_setDecimalOne(){
    LCDMEMW[3] |= 0x0100;
    LCDBMEMW[3] |= 0x0100;

 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearDecimalOne()

  DESCRIPTION :
                Disables the decimal icon on the LCD between characters 2 and 3

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the decimal icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearDecimalOne(){
    LCDMEMW[3] &= ~0x0100;
    LCDBMEMW[3] &= ~0x0100;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setDecimalTwo()

  DESCRIPTION :
                Enables the decimal icon on the LCD between characters 4 and 5

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the location of the decimal icon so it enables
*F ---------------------------------------------------------------------------*/
    void LCD_setDecimalTwo(){
    LCDMEMW[5] |= 0x0100;
    LCDBMEMW[5] |= 0x0100;

 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearDecimalTwo()

  DESCRIPTION :
                Disables the decimal icon on the LCD between characters 4 and 5

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the location of the decimal icon so it disables
*F ---------------------------------------------------------------------------*/
  void LCD_clearDecimalTwo(){
    LCDMEMW[5] &= ~0x0100;
    LCDBMEMW[5] &= ~0x0100;
 }
 
 /*F ----------------------------------------------------------------------------
  NAME :      LCD_setDecimals()

  DESCRIPTION :
                Enables the decimal icons on the LCD between characters 2 and 3 & 4 and 5

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "1" in the locations of the decimal icons so they enable
*F ---------------------------------------------------------------------------*/
 void LCD_setDecimals(){
    LCDMEMW[5] |= 0x0100;
    LCDBMEMW[5] |= 0x0100;
    LCDMEMW[3] |= 0x0100;
    LCDBMEMW[3] |= 0x0100;

 }

 /*F ----------------------------------------------------------------------------
  NAME :      LCD_clearDecimals()

  DESCRIPTION :
                Disables the decimal icons on the LCD between characters 2 and 3 & 4 and 5

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a "0" in the locations of the decimal icons so they disable
*F ---------------------------------------------------------------------------*/
  void LCD_clearDecimals(){
    LCDMEMW[5] &= ~0x0100;
    LCDBMEMW[5] &= ~0x0100;
    LCDMEMW[3] &= ~0x0100;
    LCDBMEMW[3] &= ~0x0100;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_ClearNums()

  DESCRIPTION :
                Removes all numbers from the display by writing a blank digit to the screen in each position
                The symbol characters state's are maintained

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Set the LCD to display a " " in the number locations one by one
                [2]     The LCD one character at a time becomes blank - This progess is invisible to the human eye
*F ---------------------------------------------------------------------------*/
 void LCD_ClearNums(){
  showChar(' ', pos1);
  showChar(' ', pos2);
  showChar(' ', pos3);
  showChar(' ', pos4);
  showChar(' ', pos5);
  showChar(' ', pos6);
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_setBlink()

  DESCRIPTION :
                Sets a specified character to start blinking by toggling on and off

  INPUTS :      
                int position : The position of the character (1-6) that needs to start blinking

  RETURNS :     void

  PROCESS :
                [1]     Get the memory location of the LCD character that needs to blink
                [2]     Set half of the memory blank so toggling between the two give a blinking effect
*F ---------------------------------------------------------------------------*/
 void LCD_setBlink(int position){
    position = posLookUp[position-1];
    LCDBMEMW[position/2] = 0;
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_clearBlink()

  DESCRIPTION :
                Sets a specified character to stop blinking 

  INPUTS :      
                int position : The position of the character (1-6) that needs to stop blinking

  RETURNS :     void

  PROCESS :
                [1]     Get the memory location of the LCD character that needs to stop blinking
                [2]     Copy the half of the memory with the stored information to the blank memory to create a static number
*F ---------------------------------------------------------------------------*/
void LCD_clearBlink(int position){
    position = posLookUp[position-1];
    LCDBMEMW[position/2] = LCDMEMW[position/2];
 }

/*F ----------------------------------------------------------------------------
  NAME :      LCD_INIT()

  DESCRIPTION :
                Initialise the LCD so that it can be used to display information.
                This function needs to be called before any of the others in this file.

  INPUTS :      void

  RETURNS :     void

  PROCESS :
                [1]     Disable the watchdog time
                [2]     Setup the crystal oscillator to allow toggling of two number states
                [3]     Configure the LCD pinout
                [4]     Enable the charge pump
                [5]     Configure the LCD
                [6]     Clear the previous memory
                [7]     Divide the clock to give a slower flashing effect
                [8]     Enable writing to the LCD     
*F ---------------------------------------------------------------------------*/

void LCD_INIT( void )
{
    WDTCTL = WDTPW | WDTHOLD;                                  // Stop watchdog timer

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2;                                     // P4.2~P4.1: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                         // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                                  // Test oscillator fault flag
    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2;            // Higher drive strength and current consumption for XT1 oscillator


    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LCD pins
    SYSCFG2 |= LCDPCTL;                                        // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

    LCDPCTL0 = 0xFFFF;
    LCDPCTL1 = 0x07FF;
    LCDPCTL2 = 0x00F0;                                         // L0~L26 & L36~L39 pins selected

    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                            // flcd ref freq is xtclk

    //LCD Operation - Mode 2
    LCDVCTL = LCDCPEN |                                        //Enable charge pump
            LCDSELVDD |                                        //Internally connect to Vcc
            VLCD_8 |                                           //internal 3.08v
           (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);//Charge pump frequency selection 256Hz

    LCDMEMCTL |= LCDCLRM | LCDCLRBM;                           // Clear LCD memory

    LCDCSSEL0 = 0x000F;                                        // Configure COMs and SEGs
    LCDCSSEL1 = 0x0000;                                        // L0, L1, L2, L3: COM pins
    LCDCSSEL2 = 0x0000;

    LCDM0 = 0x21;                                              // L0 = COM0, L1 = COM1
    LCDM1 = 0x84;                                              // L2 = COM2, L3 = COM3

    LCDBM0 = 0x21;
    LCDBM1 = 0x84;

    LCDBLKCTL = LCDBLKPRE__32 |                               //Divide xtclk by 512
            LCDBLKMOD_3;                                       //Switch between memory contents of LCDM and LCDB

    LCDCTL0 |= LCD4MUX | LCDON;                                // Turn on LCD, 4-mux selected

    PMMCTL0_H = PMMPW_H;                                       // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF_L;                                  // and set PMMREGOFF
}

/*F ----------------------------------------------------------------------------
  NAME :      LCD_WriteAll()

  DESCRIPTION :
                A singular function to write to the LCD which displays characters (1-6)

  INPUTS :      
                char[6] : Takes in six characters which will be displayed in order of being passed

  RETURNS :     void

  PROCESS :
                [1]     Take the first input to the function and write it to the first position on the LCD
                [2]     Take the second input to the function and write it to the second position on the LCD
                [3]     This process is the same for characters 3-6 
*F ---------------------------------------------------------------------------*/
void LCD_WriteAll(char text1, char text2, char text3, char text4, char text5, char text6){
    showChar(text1, pos1);
    showChar(text2, pos2);
    showChar(text3, pos3);
    showChar(text4, pos4);
    showChar(text5, pos5);
    showChar(text6, pos6);
}

/*F ----------------------------------------------------------------------------
  NAME :      LCD_WriteSingle()

  DESCRIPTION :
                Writes a provided character to a given LCD location

  INPUTS :      
                char    : The character to be displayed on the LCD
                int     : The LCD position number to be written too (1-6)

  RETURNS :     void

  PROCESS :
                [1]     Using the position data use a look up to identify the LCD memory location to write to
                [2]     Call the function showChar to display the character at the found memory location
*F ---------------------------------------------------------------------------*/
void LCD_WriteSingle(char text, int position){
    position = posLookUp[position-1];
    showChar(text, position);
}
