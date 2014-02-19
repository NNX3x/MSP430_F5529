#ifndef DEVIO_H_
#define DEVIO_H_

#include "configs.h"

/******************************************************************************
 * IO Ports
 *****************************************************************************/
/* UART Lines*/
//#define DEVIO_P1IN_RTS 			(0x01 << 0)
#define DEVIO_P1IN_RXD			(0x01 << 1)
#define DEVIO_P1OUT_TXD			(0x01 << 2)
#define DEVIO_P1OUT_CTS 		(0x01 << 3)

/* SPI Lines */
#define DEVIO_P3OUT_SCK			(0x01 << 2)
#define DEVIO_P3IN_MISO			(0x01 << 1)
#define DEVIO_P3OUT_MOSI		(0x01 << 0)

/* Status LED */
#define DEVIO_P1OUT_STAT_LED 	(0x01 << 0)

/******************************************************************************
 * Global functions
 *****************************************************************************/
void DEVIO_Init(void);
void DEVIO_StatusLED_Set(uint8_t state);

#endif /* DEVIO_H_ */
