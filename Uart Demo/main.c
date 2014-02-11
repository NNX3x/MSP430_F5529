#include <msp430.h>
#include <stdint.h>
#include "uart.h"

#define led_2 BIT7
#define led_1 BIT0

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P3SEL = BIT3 + BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 6;                              // 1MHz 9600 (see User's Guide)
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
											  // over sampling
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

	P1DIR |= led_1;                  //transmit led on dev board
	P4DIR |= led_2;


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
