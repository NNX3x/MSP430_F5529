
#include "configs.h"
#include "uart.h"

#define led_2 BIT7
#define led_1 BIT0

/******************************************************************************
 * Static Functions
 *****************************************************************************/
static inline uint8_t nibbleToHex(uint8_t nib);

/******************************************************************************
 * Global variables
 *****************************************************************************/
volatile uint8_t UART_DataReady;
volatile uint8_t UART_DataRX;

/******************************************************************************
 * Global functions
 *****************************************************************************/

/*-----------------------------------------------------------------------------
 * Function name:   UART_Init
 * Description:     Initializes the UART routines on USCIA0
 * Parameters:      NONE
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void UART_Init(void)
{
	//pg 909
	UART_DataReady = FALSE;
	UART_DataRX = 0;

	P3SEL = BIT3 + BIT4;                    // P3.4 & P3.5 = USCI_A0 TXD/RXD

    UCA0CTL1 |= UCSSEL_2; 					// SMCLK
    #ifndef FAST_CLOCK
    	UCA0BR0 = 52;                      // 8MHz 9600
    	UCA0BR1 = 0;
    	UCA0MCTL = UCBRS_0 + UCBRF_1 + UCOS16;         // Modulation UCBRSx = 2 UCBRFx=0 & oversampling
    #else
        UCA0BR0 = 0xEC;                      // 12.1MHz - 9600
        UCA0BR1 = 0x04;
        UCA0MCTL = UCBRS_0 + UCBRF_0;         // Modulation UCBRSx = 0 UCBRFx=0
    #endif

  	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
  	UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void UART_SendByte(uint8_t data)
{

	while (!(UCA0IFG&UCTXIFG)) ; // Make sure it isn't already tx'ing
	 UCA0TXBUF = data;
	P1OUT^=led_1;
}


/*-----------------------------------------------------------------------------
 * Function name:   UART_SendBuffer
 * Description:     Sends an entire buffer out on the UART
 * Parameters:      buffer - Buffer to send
 *                  length - Number of bytes in the buffer to send
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void UART_SendBuffer(uint8_t* buffer, uint8_t length)
{
    uint8_t i;

    for(i = 0; i < length; i++)
    {
        UART_SendByte(buffer[i]);
    }
}


/*-----------------------------------------------------------------------------
 * Function name:   UART_SendString
 * Description:     Sends out a NULL terminated string on the UART
 * Parameters:      str - NULL terminated string pointer
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void UART_SendString(const char* str)
{
    while(*str != '\0')
    {
        UART_SendByte((uint8_t)(*str));
        str++;
    }
}


/*-----------------------------------------------------------------------------
 * Function name:   UART_SendHexByte
 * Description:     Sends a byte out on the UART in a human readable (ASCII)
 *                  format. (1 byte of data in, 2 bytes of ASCII out)
 * Parameters:      data - byte to send as human readable hex
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void UART_SendHexByte(uint8_t data)
{
    UART_SendByte(nibbleToHex(data >> 0x04));
    UART_SendByte(nibbleToHex(data & 0x0F));
}


/*-----------------------------------------------------------------------------
 * Function name:   nibbleToHex
 * Description:     Converts the lower four bits (nibble) of the given byte
 *                  into its ASCII equivalent hex character.
 * Parameters:      nib - nibble to convert
 * Returns:         Character representation
 *---------------------------------------------------------------------------*/
static inline uint8_t nibbleToHex(uint8_t nib)
{
    if(nib > 9)
    {
        return (nib + 'A' - 10);
    }
    return (nib + '0');
}


