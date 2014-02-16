#include "configs.h"

#define led_2 BIT7
#define led_1 BIT0

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	DEVIO_Init();

	P2DIR |= BIT2;                            // SMCLK set out to pins
	P2SEL |= BIT2;
	P7DIR |= BIT7;                            // MCLK set out to pins
	P7SEL |= BIT7;

	P1DIR |= led_1;                  //transmit led on dev board
	P4DIR |= led_2;
	UART_Init();

	//UART_SendString("Goliath Online!");

	__bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
    P1OUT^=led_1;
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
