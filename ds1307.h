/*
 * @file    ds1307.h
 * @brief   I2C RTC DS1307 library for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#ifndef DS1307_H_
#define DS1307_H_

/*** TERMINAL LIBRARIES ***/
#include "i2c.h"


/*** DS1307 DEFINES ***/
#define I2C_BAUDRATE 100000U

// I2C0 PINS
#define I2C0_PORT 'B'
#define I2C0_SDA 3
#define I2C0_SCL 2

#define I2C_MASTER_CLK_SRC          I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define WAIT_TIME                   10U

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x68U
#define I2C_DATA_LENGTH            7U



#endif /* DS1307_H_ */
