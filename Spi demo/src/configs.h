/*
 * configs.h
 *
 *  Created on: Feb 9, 2014
 *      Author: Braden
 */

#ifndef CONFIGS_H_
#define CONFIGS_H_

/******************************************************************************
 * System includes
 *****************************************************************************/
#include <msp430.h>
#include <stdint.h>

/******************************************************************************
 * Local includes
 *****************************************************************************/
#include "devio.h"
//#include "uart.h"
#include "spi.h"
#include "spi_configs.h"
/******************************************************************************
 * Defines
 *****************************************************************************/
#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif
#ifndef NULL
    #define NULL ((void *)0)
#endif

//#define FAST_CLOCK
/* Use 12MHz core clock instead of the default 1MHz clock.
 *  ****** devio does 12 Mhz and 8 Mhz only. ********
 */

#endif /* CONFIGS_H_ */
