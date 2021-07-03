/*
 * @file    ds1307.h
 * @brief   I2C RTC DS1307 library for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#include "ds1307.h"

/*** I2C VARIABLES ***/
uint8_t I2C_master_txBuff[I2C_DATA_LENGTH] = { 0x80, 0x22, 0x11, 0x00, 0x12, 0x06, 0x21 };
uint8_t I2C_master_rxBuff[I2C_DATA_LENGTH];
