/*
 * @file    ds1307.h
 * @brief   I2C RTC DS1307 library for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#include "ds1307.h"

/*** I2C VARIABLES ***/
//uint8_t I2C_master_rxBuff[DS1307_DATA_LENGTH];

status_t sentPackageI2C(bool timeDateSel, const uint8_t *dataArray) {
	status_t statusValue;
	uint8_t txBuffer[DS1307_DATA_LENGTH] = { 0 };
	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	if (!timeDateSel) {
		masterXfer.subaddress = (uint32_t) DS1307_TIME_WADDR;
		transTime(dataArray, txBuffer);
	} else {
		masterXfer.subaddress = (uint32_t) DS1307_DATE_WADDR;
		transDate(dataArray, txBuffer);
	}

	/* 	subAddress = 0x01, data = g_master_txBuff - write to slave.
	 start + slaveaddress(w) + subAddress + length of data buffer
	 + data buffer + stop
	 */
	masterXfer.slaveAddress = DS1307_SLAVE_ADDR_7BIT;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddressSize = 1;
	masterXfer.data = txBuffer;
	masterXfer.dataSize = DS1307_DATA_LENGTH;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	statusValue = I2C_MasterTransferBlocking(I2C0, &masterXfer);
	// Wait for the slave to process the data
	for (uint32_t i = 0U; i < WAIT_TIME; i++)
	{
		__NOP();
	}
	return statusValue;
}

status_t receivePackageI2C(bool timeDateSel, uint8_t *dataArray){
	status_t statusValue;
	uint8_t rxBuffer[DS1307_DATA_LENGTH] = { 0 };
	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	(!timeDateSel) ? (masterXfer.subaddress = (uint32_t) DS1307_TIME_WADDR) : (masterXfer.subaddress = (uint32_t) DS1307_DATE_WADDR);

	masterXfer.slaveAddress = DS1307_SLAVE_ADDR_7BIT;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddressSize = 1;
	masterXfer.data = rxBuffer;
	masterXfer.dataSize = DS1307_DATA_LENGTH;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	statusValue = I2C_MasterTransferBlocking(I2C0, &masterXfer);

	(!timeDateSel) ? (timeBack2Char(dataArray, rxBuffer)) : (dateBack2Char(dataArray, rxBuffer));

	return statusValue;
}

void timeBack2Char (uint8_t *mainData, uint8_t *i2cData){
	uint8_t seg = 0, min = 0, hr = 0;
	seg = i2cData[0];
	min = i2cData[1];
	hr = i2cData[2];

	mainData[0] = (hr / 10) + 48;
	mainData[1] = (hr % 10) + 48;
	mainData[2] = (uint8_t) ':';
	mainData[3] = (min / 10) + 48;
	mainData[4] = (min % 10) + 48;
	mainData[5] = (uint8_t) ':';
	seg &= 0x7F;
	mainData[6] = (seg / 10) + 48;
	mainData[7] = (seg % 10) + 48;
}

void dateBack2Char (uint8_t *mainData, uint8_t *i2cData){
	uint8_t year = 0, month = 0, day = 0;
	day = i2cData[0];
	month = i2cData[1];
	year = i2cData[2];

	mainData[0] = (day / 10) + 48;
	mainData[1] = (day % 10) + 48;
	mainData[2] = (uint8_t) '/';
	mainData[3] = (month / 10) + 48;
	mainData[4] = (month % 10) + 48;
	mainData[5] = (uint8_t) '/';
	mainData[6] = (year / 10) + 48;
	mainData[7] = (year % 10) + 48;
}

void transTime(const uint8_t *charData, uint8_t *i2cData) {
	uint8_t seg = 0, min = 0, hr = 0;
	uint8_t bcdLS = 0, bcdMS = 0;

	if ((charData[2] == ':') && (charData[5] == ':')) {
		seg = charData[6] - 48;
		seg *= 10;
		seg += (charData[7] - 48);

		min = charData[3] - 48;
		min *= 10;
		min += (charData[4] - 48);

		hr = charData[0] - 48;
		hr *= 10;
		hr += (charData[1] - 48);

		if ((seg >= 0) && (seg < 60)) {
			bcdLS = seg % 10;
			bcdMS = seg / 10;
			seg = bcdMS << 4;
			seg |= bcdLS;
			seg |= 0x80;
		} else {
			seg = 0x80;
		}
		if ((min >= 0) && (min < 60)) {
			bcdLS = min % 10;
			bcdMS = min / 10;
			min = bcdMS << 4;
			min |= bcdLS;
		} else {
			min = 0x00;
		}
		if ((hr >= 0) && (hr < 24)) {
			bcdLS = hr % 10;
			bcdMS = hr / 10;
			hr = bcdMS << 4;
			hr |= bcdLS;
			hr &= 0x3F;
		} else {
			hr = 0x00;
		}
	}
	i2cData[0] = seg;
	i2cData[1] = min;
	i2cData[2] = hr;
}

void transDate(const uint8_t *charData, uint8_t *i2cData) {
	uint8_t year = 0, month = 0, day = 0;
	uint8_t bcdLS = 0, bcdMS = 0;

	if ((charData[2] == '/') && (charData[5] == '/')) {
		year = charData[6] - 48;
		year *= 10;
		year += (charData[7] - 48);

		month = charData[3] - 48;
		month *= 10;
		month += (charData[4] - 48);

		day = charData[0] - 48;
		day *= 10;
		day += (charData[1] - 48);

		if ((year >= 0) && (year < 100)) {
			bcdLS = year % 10;
			bcdMS = year / 10;
			year = bcdMS << 4;
			year |= bcdLS;
		} else {
			year = 0x00;
		}
		if ((month > 0) && (month < 13)) {
			bcdLS = month % 10;
			bcdMS = month / 10;
			month = bcdMS << 4;
			month |= bcdLS;
		} else {
			month = 0x01;
		}
		if ((day > 0) && (day < 32)) {
			bcdLS = day % 10;
			bcdMS = day / 10;
			day = bcdMS << 4;
			day |= bcdLS;
			day &= 0x3F;
		} else {
			day = 0x01;
		}
	}
	i2cData[0] = day;
	i2cData[1] = month;
	i2cData[2] = year;
}


