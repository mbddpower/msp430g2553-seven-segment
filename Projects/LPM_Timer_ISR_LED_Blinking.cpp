
#include "io430.h"

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void){
  
 P1OUT ^= 0x41; 
 TA0CCR0 += 1500;
}

int main( void )
{
  P1OUT = 0x01;
  P2OUT = 0x00;
  P1DIR = 0xFF;
  P2DIR = 0x00;
  
  TA0CCR0= 62500;
  TA0CTL = TASSEL_2 + ID_3 + MC_1;
  TA0CCTL0_bit.CCIE = 1;
  
  WDTCTL = WDTPW + WDTHOLD; 
  __bis_SR_register(LPM0_bits + GIE);
 
}
