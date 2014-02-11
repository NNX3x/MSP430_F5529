#include <msp430.h>
#include <stdint.h>
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


