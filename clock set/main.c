#include "configs.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1DIR |= BIT0;                            // P1.1 output
    DEVIO_Init();

	while(1)
	  {
	    P1OUT ^= BIT0;                          // Toggle P1.1
	    __delay_cycles(600000);                 // Delay
	  }
}
