/*
 * @file    i2c.c
 * @brief   I2C configuration for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#include "i2c.h"

bool I2C0_Init(uint8_t port, uint8_t sdaPin, uint8_t clkPin) {
	/*** I2C CONFIG INIT ***/
	const port_pin_config_t I2C_pin_config = {
		kPORT_PullUp, /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate, /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable, /* Passive filter is disabled */
		kPORT_OpenDrainEnable, /* Open drain is enabled */
		kPORT_LowDriveStrength, /* Low drive strength is configured */
		kPORT_MuxAlt2, /* Pin is configured as I2C0_SCL or I2C_SDA */
		kPORT_UnlockRegister /* Pin Control Register fields [15:0] are not locked */
	};
	bool i2c0_OK = 1;

	if (port == 'B') {
		CLOCK_EnableClock(kCLOCK_PortB); /* Port B Clock Gate Control: Clock enabled */
		if (sdaPin == 3U) {
			PORT_SetPinConfig(PORTB, sdaPin, &I2C_pin_config); /* PORTB3 is configured as I2C0_SDA */
		} else {
			i2c0_OK = 0;
			return i2c0_OK;
		}
		if (clkPin == 2U) {
			PORT_SetPinConfig(PORTB, clkPin, &I2C_pin_config); /* PORTB2 is configured as I2C0_SCL */
		} else {
			i2c0_OK = 0;
			return i2c0_OK;
		}
	}else{
		i2c0_OK = 0;
	}
	return i2c0_OK;
}

void I2C0_Config(uint32_t baudrate){
	i2c_master_config_t I2CmasterConfig;

	/*
	 * masterConfig->baudRate_Bps = 100000U;
	 * masterConfig->enableStopHold = false;
	 * masterConfig->glitchFilterWidth = 0U;
	 * masterConfig->enableMaster = true;
	 */
	I2C_MasterGetDefaultConfig(&I2CmasterConfig);
	I2CmasterConfig.baudRate_Bps = baudrate;
	I2C_MasterInit(I2C0, &I2CmasterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));

}
