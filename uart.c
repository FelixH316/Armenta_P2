/*
 * @file    uart.c
 * @brief   UART configuration for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#include "uart.h"

bool UART0_Init(uint8_t port, uint8_t txPin, uint8_t rxPin) {
	bool uart0_OK = 1;
	if (port == 'B') {
		CLOCK_EnableClock(kCLOCK_PortB); /* Port B Clock Gate Control: Clock enabled */

		if (txPin == 17U) {
			PORT_SetPinMux(PORTB, txPin, kPORT_MuxAlt3); /* PORTB17 (pin 63) is configured as UART0_TX */
		} else {
			uart0_OK = 0;
			return uart0_OK;
		}

		if (rxPin == 16U) {
			PORT_SetPinMux(PORTB, rxPin, kPORT_MuxAlt3); /* PORTB16 (pin 62) is configured as UART0_RX */
		} else {
			uart0_OK = 0;
			return uart0_OK;
		}

		SIM->SOPT5 = ((SIM->SOPT5 & (~(SIM_SOPT5_UART0TXSRC_MASK))) /* Mask bits to zero which are setting */
		| SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX) /* UART 0 transmit data source select: UART0_TX pin */
		);
	} else {
		uart0_OK = 0;
	}
	return uart0_OK;
}

void UART0_Config(uint32_t baudrate) {
	/*** UART0 SETTINGS ***/
	uart_config_t UARTconfig;
	/*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&UARTconfig);
	UARTconfig.baudRate_Bps = baudrate;
	UARTconfig.enableTx = true;
	UARTconfig.enableRx = true;
	UART_Init(UART0, &UARTconfig, CLOCK_GetFreq(UART0_CLK_SRC));

	// Enable UART0_RX ISR
	UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	EnableIRQ(UART0_RX_TX_IRQn);
}

