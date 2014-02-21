/******************************************************************************
 *
 * TODO: License and terms, NDA, etc...
 *
 * FILE_NAME_
 * FILE_FUNCTION / DESCRIPTION
 *****************************************************************************/
#include "configs.h"


/******************************************************************************
 * Defines
 *****************************************************************************/

#define DUMMY_BYTE (0x00)
/******************************************************************************
 * Global functions
 *****************************************************************************/


/*-----------------------------------------------------------------------------
 * Function name:   SPI_Init
 * Description:     Initializes the SPI routines on USCIB0
 * Parameters:      NONE
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void SPI_Init(void)
{

	P3SEL |= BIT0 + BIT1 + BIT2;

	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master

	// UCMSB Most significant bit first
	// UCCKPH Accept data on a rising edge.
	// UCMST Master Mode
	// UCSYNC Synchronous mode

	UCB0CTL1 |= UCSSEL_2;           	// SMCLK

	UCB0BR0 |= SPI_CLK_SPEED_SLOW;      // ex: 1MHZ / 10 = 100khz SCLK
	UCB0BR1 = 0x00;
	UCB0CTL1 &= ~UCSWRST;              // **Initialize USCI state machine**


}


/*-----------------------------------------------------------------------------
 * Function name:   SPI_SendRawByte
 * Description:     Sends a byte out on the SPI lines. This function does not
 *                  modify the SPI select line
 * Parameters:      data - data byte to send
 * Returns:         byte received while sending out data
 *---------------------------------------------------------------------------*/
uint8_t SPI_SendRawByte(uint8_t data)
{
        while (!(UCTXIFG));
        UCB0TXBUF = data;               // Send Data

        while (!(UCRXIFG));    // Wait to receive a byte
        return UCB0RXBUF;

}


/*-----------------------------------------------------------------------------
 * Function name:   SPI_SendByte
 * Description:     Sends a byte out on the SPI lines. This function does
 *                  modify the SPI select line.
 * Parameters:      data - data byte to send
 * Returns:         byte received while sending out data
 *---------------------------------------------------------------------------*/
uint8_t SPI_SendByte(uint8_t data)
{
    uint8_t ret;

   // NRF_SPI_ENABLE;
    ret = SPI_SendRawByte(data);
 //   NRF_SPI_DISABLE;

    return ret;
}

/*-----------------------------------------------------------------------------
 * Function name:   SPI_SendBuffer
 * Description:     Shifts out a buffer on the SPI lines. Command byte first,
 *                  followed by buffer[0], then buffer[1], ....
 *                  The first byte returned is the received byte from the
 *                  command, followed by the results of the buffer sending.
 * Parameters:      command - First byte to send, the command byte.
 *                  buffer - Pointer to the buffer to send out.
 *                  length - Number of bytes in the buffer to send out
 *                  ret - Return buffer pointer. This can either be NULL, or it
 *                      needs to be length +1.
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void SPI_SendBuffer(uint8_t command, uint8_t* buffer, uint8_t length, uint8_t* ret)
{
    uint8_t i;
    uint8_t retByte;

    retByte = SPI_SendRawByte(command); // Send first byte, the command
    if(ret != NULL)
    {
        ret[0] = retByte;
    }

    for(i = 0; i < length; i++)     // Send buffer
    {
        retByte = SPI_SendRawByte(buffer[i]);
        if(ret != NULL)
        {
            ret[i +1] = retByte;
        }
    }

}


/*-----------------------------------------------------------------------------
 * Function name:   SPI_GetBuffer
 * Description:     Sends out a command on the SPI lines, followed by num_bytes
 *                  dummy bytes, and returns the received bytes. This is useful
 *                  for reading large blocks of memory on the target device.
 *                  Returned result does not include the byte shifted in from
 *                  the command byte.
 * Parameters:      command - First byte to send, the command byte.
 *                  ret - Return buffer pointer. This is where the results are
 *                      stored. It must be at least num_bytes large.
 *                  num_bytes - Number of bytes to shift in.
 * Returns:         NONE
 *---------------------------------------------------------------------------*/
void SPI_GetBuffer(uint8_t command, uint8_t* ret, uint8_t num_bytes)
{
    uint8_t i;

    (void) SPI_SendRawByte(command);

    for(i = 0; i < num_bytes; i++)
    {
       ret[i] = SPI_SendRawByte(DUMMY_BYTE);
    }
}
