#include "LCD.h"
#include <msp430.h>
#include "chrono.h"
#include "clock.h"
#include "intrinsics.h"
#include "msp430fr4133.h"
#include "time.h"
#include "alarm.h"

// defines to make it a little easier
// we only need unsigned integers
#define LONG unsigned long
#define WORD unsigned short
#define BYTE unsigned char

int STARTSTOP_PRESSED = 0;


void clearSTARTSTOP(){
  P4OUT &= ~0x01;                 // Clear P4.0 (Green LED)
  STARTSTOP_PRESSED = 0;
}

int getSTARTSTOP(){
  return STARTSTOP_PRESSED;
}
/*F ----------------------------------------------------------------------------
  NAME :      green_led()

  DESCRIPTION :
              toggles the green LED

*F ---------------------------------------------------------------------------*/


/* -----------------------------------------------------------------------------
  static memory allocation for the process control blocks PCB
  each PCB consists of
    sp                  the stack pointer
    stack[STACK_SIZE]   the run time stack

  allocate memory for a fixed no. of PCBs
------------------------------------------------------------------------------*/
#define MAX_PROCESSES   4
#define STACK_SIZE      100

struct ProcessControlBlock
{
    LONG sp;
    BYTE stack[STACK_SIZE];
};

struct ProcessControlBlock process[MAX_PROCESSES];

/* -----------------------------------------------------------------------------
  first process to run is PCB[0]
------------------------------------------------------------------------------*/
unsigned int current_process = 0;

/* -----------------------------------------------------------------------------
  initialize_process needs temporary access to memory to store information
  these need to be declared global so we can use move instructions
    SREG, SP, PC, saved SP
    pc1,pc2  partial PC as saved on the stack
------------------------------------------------------------------------------*/
LONG status;
LONG stack_pointer;
LONG program_counter;
LONG saved_sp;

WORD pc1;
WORD pc2;


/*F ----------------------------------------------------------------------------
  NAME :      initialize_process()

  DESCRIPTION :
              populates the PCB

  INPUTS :    unsigned int process_index    the no (or index) of the PCB
              void (*funct)()               pointer to process

  RETURNS :   void

  PROCESS :
              [1]   store SREG into status
              [2]   calculate the stack_pointer for PCB to point at end of stack
              [3]   get the starting address of the process
              [4]   combine PC+SR used by the ISR into pc2 and pc1
              [5]   saved_sp <= sp
              [6]   sp <= stack_pointer
              [7]   push pc1
              [8]   push pc2
              [9]   push a 0 13 times
              [10]  stack_pointer <= sp
              [11]  sp <= saved_sp
              [12]  save stack_pointer into the PCB stack pointer

*F ---------------------------------------------------------------------------*/
void initialise_process(unsigned int process_index, void (*funct)())
{
    if (process_index < MAX_PROCESSES)
    {
        asm(
                " movx.a SR,&status\n"
            );

        stack_pointer = (LONG)&process[process_index] + STACK_SIZE - 2;
        program_counter = (LONG)funct;

        // Construct combined PC+SR used by interrupt

        pc1 = (WORD)program_counter;
        pc2 = (WORD)(((program_counter>>4)&0x0F000) | status&0x00FFF);

        asm(
                " movx.a sp,&saved_sp\n"
                " movx.a &stack_pointer,sp\n"
                " push.w &pc1\n"
                " push.w &pc2\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " push.a #0\n"
                " movx.a sp,&stack_pointer\n"
                " movx.a &saved_sp,sp\n"
            );

        process[process_index].sp = stack_pointer;
    }
}

/*F ----------------------------------------------------------------------------
  NAME :      run_process()

  DESCRIPTION :
              runs the process

  INPUTS :    unsigned int process_index    the no (or index) of the PCB

  RETURNS :   void

  PROCESS :
              [1]   if valid process
              [2]     stack_pointer <= stack pointer of the PCB
              [3]     sp <= stack_pointer
              [4]     pop R3 - R15
              [5]     RETI

*F ---------------------------------------------------------------------------*/
void run_process(unsigned int process_index)
{
    if (process_index < MAX_PROCESSES)
    {
        stack_pointer = process[process_index].sp;

        asm(
                " movx.a &stack_pointer,SP \n"
                " pop.a R3 \n"
                " pop.a R4 \n"
                " pop.a R5 \n"
                " pop.a R6 \n"
                " pop.a R7 \n"
                " pop.a R8 \n"
                " pop.a R9 \n"
                " pop.a R10 \n"
                " pop.a R11 \n"
                " pop.a R12 \n"
                " pop.a R13 \n"
                " pop.a R14 \n"
                " pop.a R15 \n"
                " RETI \n"
        );
    }
}



/*F ----------------------------------------------------------------------------
  NAME :      main()

  DESCRIPTION :
              main program

  INPUTS :    none

  RETURNS :   void

  PROCESS :
              [1]   set up pins
              [2]   set up timer interrupt
              [3]   initialize both processes
              [4]   start first process
              [5]   enable interrupts
              [6]   do forever
              [7]     red_led()
              [8]     green_led()

*F ---------------------------------------------------------------------------*/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    // Initialisation

    // Initialisation - Hardware

    PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode
                                    // to activate previously configured port settings

    P1DIR |=  0x01;                 // Set P1.0 to output direction
    P4DIR |=  0x01;                 // Set P4.6 to output direction
    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)
    P4OUT &= ~0x01;                 // Set P4.6 off (Red LED)

  // set SW1 as GPIO input with pullup
    #define SW1 2
    P1SEL0 &= ~(1<<SW1);
    P1OUT |= (1<<SW1);
    P1REN |= (1<<SW1);
    P1DIR &= ~(1<<SW1);

    P1IE  |= 0x04;  // Enable interrupt on P1.2
    P1IES |= 0x04;  // Set P1.2 button interrupt to be a high-to-low tranisition
    P1IFG &= ~0x04; // Clear local interrupt flag for P1.2


    #define SW2 6
    P2SEL0 &= ~(1<<SW2);
    P2OUT |= (1<<SW2);
    P2REN |= (1<<SW2);
    P2DIR &= ~(1<<SW2);

    P2IE  |= 0x40;  // Enable interrupt on P2.6
    P2IES |= 0x40;  // Set P2.6 button interrupt to be a high-to-low tranisition
    P2IFG &= ~0x40; // Clear local interrupt flag for P2.6
    

                                    // Timer A0 (1ms interrupt)
    TA0CCR0 =  1024;                // Count up to 1024
    //TA0CCTL0 = 0x10;                // Enable counter interrupts, bit 4=1
    TA0CTL =  TASSEL_2 + MC_1;      // Timer A using subsystem master clock, SMCLK(1.1 MHz)
                                    // and count UP to create a 1ms interrupt


    LCD_INIT();
    LCD_WriteAll('1','2','D','Z','A','O');
    LCD_setBlink(1);
    LCD_clearBlink(1);
    LCD_WriteSingle('F', 6);

    _BIS_SR(GIE);                   // interrupts enabled

    // Initialisation - Software

    initialise_process(0, clock);
    initialise_process(1, time);
    initialise_process(2, alarm);
    initialise_process(3, chrono);


    run_process(current_process);




    for (;;)
    {

    }
}


/*F ----------------------------------------------------------------------------
  NAME :      Timer0_A0()

  DESCRIPTION :
              ISR for the timer0_A0 for interrupt

  INPUTS :    none

  RETURNS :   void

  PROCESS :
              [1]   save contexxt on PCB stack
              [2]   stack_pointer <= sp
              [3]   save stack_pointer in PCB sp
              [4]   get next PCB
              [5]   get PCB sp into stack_pointer
              [6]   sp <= stack_pointer
              [7]   get context from stack

*F ---------------------------------------------------------------------------*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void)
{
    __disable_interrupt();
    __delay_cycles(40000);

    if (!(P1IN & BIT2)) { // Check again if switch is still pressed
            // Save first process details...
      P1OUT ^= 0x01;                 // Set P1.0 toggle (Green LED)

     asm(
              " push.a R10\n"
              " push.a R9\n"
              " push.a R8\n"
              " push.a R7\n"
              " push.a R6\n"
              " push.a R5\n"
              " push.a R4\n"
              " push.a R3\n"
              " movx.a sp,&stack_pointer\n"
          );

      process[current_process].sp = stack_pointer;

      current_process = (current_process+1) % MAX_PROCESSES;

      stack_pointer = process[current_process].sp;

      asm(
              " movx.a &stack_pointer,SP \n"
              " pop.a R3 \n"
              " pop.a R4 \n"
              " pop.a R5 \n"
              " pop.a R6 \n"
              " pop.a R7 \n"
              " pop.a R8 \n"
              " pop.a R9 \n"
              " pop.a R10 \n"

      );
    }
    P1IFG &= ~0x04; // Clear local interrupt flag for P1.2
    _BIS_SR(GIE);                   // interrupts enabled

}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    __disable_interrupt();
    __delay_cycles(40000);
    P4OUT |= 0x01;                 // Set P4.0 (Green LED)
    STARTSTOP_PRESSED = 1;
    P2IFG &= ~BIT6; // Clear local interrupt flag for P2.6
    _BIS_SR(GIE);                   // interrupts enabled

}