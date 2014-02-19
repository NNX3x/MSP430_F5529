#include "configs.h"


/******************************************************************************
 * Global functions
 *****************************************************************************/


/*-----------------------------------------------------------------------------
 * Function name:   DEVIO_Init
 * Description:     Master initialization routine for all ports and driver code
 * Parameters:      NONE
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void DEVIO_Init(void)
{
	/* Set DCO frequency*/

    #ifndef FAST_CLOCK

		UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
		UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

		__bis_SR_register(SCG0);					// Disable the FLL control loop
		UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
		UCSCTL1 = DCORSEL_5;                     // Select DCO range 24MHz operation
		UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
												  // (N + 1) * FLLRef = Fdco
												  // (374 + 1) * 32768 = 12MHz
												  // Set FLL Div = fDCOCLK/2
		__bic_SR_register(SCG0);				// Enable the FLL control loop

		// Worst-case settling time for the DCO when the DCO range bits have been
		// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
		// UG for optimization.
		// 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
		__delay_cycles(375000);

	      // Loop until XT1,XT2 & DCO fault flag is cleared
	      do
	      {
	        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                                // Clear XT2,XT1,DCO fault flags
	        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	      }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

    #else

	        UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	        UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
	        UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

	        // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	        do
	        {
	          UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                                  // Clear XT2,XT1,DCO fault flags
	          SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	        }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	        __bis_SR_register(SCG0);                  // Disable the FLL control loop
	        UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
	        UCSCTL2 |= 249;                           // Set DCO Multiplier for 8MHz
	                                                  // (N + 1) * FLLRef = Fdco
	                                                  // (249 + 1) * 32768 = 8MHz
	        __bic_SR_register(SCG0);                  // Enable the FLL control loop

	        // Worst-case settling time for the DCO when the DCO range bits have been
	        // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	        // UG for optimization.
	        // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
	        __delay_cycles(250000);

    #endif
}

/*-----------------------------------------------------------------------------
 * Function name:   DEVIO_StatusLED_Set
 * Description:     Turns the status LED on or off
 * Parameters:      state - '0' off, else on
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void DEVIO_StatusLED_Set(uint8_t state)
{
    if(state != 0)
    {
        P1OUT |= DEVIO_P1OUT_STAT_LED;
    } else
    {
        P1OUT &= ~(DEVIO_P1OUT_STAT_LED);
    }
}
