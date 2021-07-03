/*
 * @file    terminal.c
 * @brief   UART Terminal 9600 Baud-rate for I2C RTC [DS1307]
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jun 13th 2021
 * @vers	v0.1
 */

#include "terminal.h"

/*** UART VARIABLES ***/
volatile uart_isr_state_t uartRxCurrentState;
volatile uint8_t menuSel[2] = { 0, 0 };

volatile bool enterFlag = false;
volatile bool escFlag = false;
volatile uint8_t UartRingBuffer[RING_BUFFER_SIZE] = {0};
volatile uint16_t rxIndex = 0;
volatile uint16_t txIndex = 0;


/*** IRS FOR RX UART0 ***/
void UART0_RX_TX_IRQHandler(void) {
	uint8_t data;
	bool ringBufferFlag = false;

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0)) {
		data = UART_ReadByte(UART0);

		switch (uartRxCurrentState) {
		case menuInput:
			if ((data > 48) && (data < 53)) {
				menuSel[0] = data - 48;
			} else if ((data == '\r') && (menuSel[0] != 0)) {
				menuSel[1] = data;
			}
			ringBufferFlag = true;
			break;
		case timeInput:
			(data == '\r') ? (enterFlag = true) : (ringBufferFlag = true);
			break;
		case dateInput:
			(data == '\r') ? (enterFlag = true) : (ringBufferFlag = true);
			break;
		case escInput:
			if (data == 0x1B) {
				escFlag = true;
			}
			break;
		}
		/* If ring buffer is not full, add data to ring buffer. */
		if (ringBufferFlag) {
			if (((rxIndex + 1) % RING_BUFFER_SIZE) != txIndex) {
				UartRingBuffer[rxIndex] = data;
				rxIndex++;
				rxIndex %= RING_BUFFER_SIZE;
			}
		}
	}
	SDK_ISR_EXIT_BARRIER;
}

void setUartIRQState(uart_isr_state_t state) {
	uartRxCurrentState = state;
}

uart_isr_state_t getUartIRQState(void) {
	return uartRxCurrentState;
}

void cleanMenuInputs(bool *flag) {
	menuSel[0] = 0;
	menuSel[1] = 0;
	*flag = true;
}

bool optionSelected(uint8_t number) {
	if ((menuSel[0] == number) && (menuSel[1] == '\r')) {
		return true;
	} else {
		return false;
	}
}

bool getEnterFlag(void){
	return enterFlag;
}

void setEnterFlag(bool val){
	enterFlag = val;
}

bool getEscFlag(void){
	return escFlag;
}

void setEscFlag(bool val){
	escFlag = val;
}

void saveArray (uint8_t* array){
	txIndex -= 8;
	for (uint8_t i = 0; i < 8; i++) {
		array[i] = UartRingBuffer[txIndex + i];
	}
}

bool diffIndex (void){
	if (rxIndex != txIndex){
		return true;
	}
	else{
		return false;
	}
}

void printFromRingBuffer (void){
	UART_WriteByte(UART0, UartRingBuffer[txIndex]);
	txIndex++;
	txIndex %= RING_BUFFER_SIZE;
}

void clearIndex (void){
	txIndex = 0;
	rxIndex = 0;
}
