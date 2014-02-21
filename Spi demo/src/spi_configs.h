/*
 * spi_configs.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Braden
 */

#ifndef SPI_CONFIGS_H_
#define SPI_CONFIGS_H_

/******************************************************************************
 * Defines
 *****************************************************************************/


#define SPI_CLK_SRC              2 			    //  1 = ACLK  2 = SMCLK
#define SPI_CLK_SPEED_FAST	   (0x04)			//	fBitClock= fBRCLK/UCBRx		16MHz/4 gives 4MHz spi clk
#define SPI_CLK_SPEED_SLOW	   (0x0A)

#endif /* SPI_CONFIGS_H_ */
