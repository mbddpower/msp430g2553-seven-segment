
#include "io430.h"

int main( void )
{
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL  = CALDCO_16MHZ;
  WDTCTL = WDTPW + WDTHOLD;
  
  P2OUT &= 0x00;  // Resistors pull down
  P2REN |= 0xFF;  // Resistors for P2
  P2DIR &= 0x00;  // P2 is Input
  P1OUT &= 0x00;  // P1 is Cleared
  P1DIR |= 0xFF;  // P1 is set as Output
  
  P2IES |= 0x01;  // (0 for low to high) - (1 for high to low) for P2.0
  P2IFG &= 0x00;  // Interrupt flags are cleared
  P2IE  |= BIT0;  // Interrupt is enabled for P2.0
  
  
  __bis_SR_register(LPM0_bits | GIE);
    __no_operation();
  
  
  while(1);
  
}


#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
/*switch(P2IV)
{
case 6:         // P1.2
  {
  P1OUT |= 0x01; 
  break;
  }
  default:break;
}
 */
  __delay_cycles(1600000);while(P2IN&0x01); // Arc damping
  P1OUT ^=  0x01; // LED on
  P2IFG &= ~0x01; // Clear interrupt flag for P2.0
  
}