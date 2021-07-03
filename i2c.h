/*
 * @file    i2c.h
 * @brief   I2C configuration for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#ifndef I2C_H_
#define I2C_H_

/*** I2C LIBRARIES ***/
#include <string.h>
#include "fsl_i2c.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "MK64F12.h"

/*** I2C0 FUNCTIONS ***/
bool I2C0_Init(uint8_t port, uint8_t sdaPin, uint8_t clkPin);
void I2C0_Config(uint32_t baudrate);

#endif /* I2C_H_ */
