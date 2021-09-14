/* 
This program controls a seven segment display directly from 
GPIOs of MSP430 microcontroller
There are two push buttons at P2.0 and P2.1 with high active 
configuration
In this code, Port Interrupt, LPM4, and Pulldown resistors 
are used
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
     P2.1 |----->      |
          |            |
----------|           [ ] 1k Pull down resistor
                      [ ]
                       |                   
                       |
                      ---/
                        /

P2.0 decreases the number shown on display by one
P2.1 increases the number shown on display by one
If the number is less than 0 "L" will be shown on the display
If the number is greater than 9 "H" will be shown on the display

*/
#include "io430.h"

short button=0;
const unsigned short num[16]={
   0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,
   0x7F,0x6F,0x5F,0x7C,0x38,0x79,0x3E,0x1E
};

int main( void )
{
 
  WDTCTL = WDTPW + WDTHOLD;

  // INITIALLY, PORTS ARE CLEARED
  P1OUT=num[0];
  P2OUT=0x00;
  // PIN DIRECTION SETTINGS
  P1DIR=0x7F; // P1.7 is Input, others all are Output
  P2DIR=0x00; // All pins are input to avoid short circuits
  // PULLUP/PULLDOWN RESISTOR SETTINGS
  P1REN=0x80; // Pull resistor is Enabled for P1.7
  P2REN=0xFF; // Pull resistors are Enabled for P2
  // INTERRUPT SETTINGS
  P2IE  = 0x03; // Interrupts are enabled
  P2IES = 0x00; // Interrupt with raising edge
  P2IFG = 0x00; // Interrupt flags are cleared
  // Global Interrupt Enable,Low Power Mode 4 Enable
  __bis_SR_register(LPM4_bits | GIE); 
 
 
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
  
  __delay_cycles(150000);
  while(P2IN&0x03);
  switch(P2IFG&0x03){
  
  case 1:button-=1;break;
  case 2:button+=1;break;
  case 3:button=0; break;
 
  }
  
  if(button>=10)P1OUT=0x5D;
  else if(button<0)P1OUT=0x34;
  else P1OUT=num[button];
  
  __delay_cycles(150000);
  P2IFG &= 0x00;
 
}
