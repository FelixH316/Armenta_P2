/*
 * @file    uart.h
 * @brief   UART configuration for NXP FRDM K64
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#ifndef UART_H_
#define UART_H_

/*** UART LIBRARIES ***/
#include <stdint.h>
#include <stdbool.h>

#include "MK64F12.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_common.h"


/*** UART0 FUNCTIONS ***/
bool UART0_Init(uint8_t port, uint8_t txPin, uint8_t rxPin);
void UART0_Config (uint32_t baudrate);

#endif /* UART_H_ */
