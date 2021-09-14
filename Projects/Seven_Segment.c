/* 
This code controls a seven segment display directly from 
GPIOs of MSP430 microcontroller
There are two push buttons at P2.0 and P2.1 with high active 
configuration
In this code, TimerA Interrupt, Port Interrupt
Low Power Mode 0, Arrays, Pulldown resistors and 
CPU Clock changing applications are used.
This schematic shows display pins and button connection
----------|
     P1.0 |-----> b
     P1.1 |-----> a
     P1.2 |-----> f
     P1.3 |-----> g
     P1.4 |-----> e
     P1.5 |-----> d               Vcc
     P1.6 |-----> c                ^
     P1.7 |                        |
          |                        |
     P2.0 |------------|----[O]-----
     P2.1 |----->      |   Button
          |            |
----------|           [ ] 1k Pull down resistor
                      [ ]
                       |                   
                       |
                      ---/
                        /

P2.0 starts and stops the timer
P2.1 increases the delay


*/
#include "io430.h"

#define LED1 P1OUT_bit.P0
#define LED2 P1OUT_bit.P6
  
  const unsigned short numbers[10]={   // ,0x5F,0x7C,0x38,0x79,0x3E,0x1E
       0x77,0x41,0x3B,0x6B,0x4D,
       0x6E,0x7E,0x43,0x7F,0x6F     };

   unsigned int  i,n,loopnumber=1;
   //_Bool stop=0;

  void initTimerInterrupt(void){

    //initialize Timer0_A
    TA0CCR0 = 62500;                  // setting up terminal count
    TA0CTL  = TASSEL_2 + ID_3 + MC_1; // configure and start timer
    
    //enable interrupts
    TA0CCTL0 |= CCIE ;                // enable timer interrupts
    __enable_interrupt();             // set GIE in SR
    
  }
  
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  initTimerInterrupt();
  TA0CTL  ^=  MC_1;
  
  P2OUT&=0x00;
  P2DIR&=0x00;
  P2REN|=0xFF;
  
  P1OUT=0x00;
  P1DIR=0x7F;
  P1REN|=0x80;

  P2IFG = 0x00;
  P2IES = 0x00;
  P2IE  = 0x03;
  
  P1OUT=numbers[i];
  __bis_SR_register(LPM0_bits+GIE);
 
  }


#pragma vector = PORT2_VECTOR
__interrupt void BUTTON(void){
  __delay_cycles(50000);
  while(P2IN&0x03);
 switch(P2IFG&0x03){
 case 1:
    TA0CTL  ^=  MC_1;           // Halt or star the counter
    TA0CTL  |=  TACLR;          // Clear the counter
    n=0;                        // Clear the external counter
    __delay_cycles(50000);
    P2IFG &= ~0x01;
    break;
 case 2:
    loopnumber++;
    //if(!loopnumber)loopnumber=1;
    __delay_cycles(50000);
    P2IFG &= ~0x02;
    break;
 }
  } 


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void)
{
    n++;
    if(n==loopnumber){
      i+=1;
      n=0;
      if(i==10)i=0;
      P1OUT=numbers[i];
    }
}
