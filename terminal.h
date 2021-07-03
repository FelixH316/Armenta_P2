/*
 * @file    terminal.h
 * @brief   UART Terminal 9600 Baud-rate for RTC DS1307
 * @author	Félix Armenta A - PADTS IECA 3
 * @date	Jul 2nd, 2021
 * @vers	v0.1
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

/*** TERMINAL LIBRARIES ***/
#include "uart.h"

/*** TERMINAL DEFINES ***/
#define RING_BUFFER_SIZE 32
#define UART_BAUDRATE 115200

// UART0 PINS
#define UART0_PORT 'B'
#define UART0_TX 17U
#define UART0_RX 16U


/*** TERMINAL TYPEDEFs ***/
typedef enum {menuInput, timeInput, dateInput, escInput} uart_isr_state_t;


/*** TERMINAL PROTOTYPES ***/
void UART0_RX_TX_IRQHandler(void);
void setUartIRQState(uart_isr_state_t state);
uart_isr_state_t getUartIRQState(void);

bool optionSelected (uint8_t number);
void cleanMenuInputs(bool *flag);
bool getEnterFlag(void);
void setEnterFlag(bool val);
bool getEscFlag(void);
void setEscFlag(bool val);
void saveArray (uint8_t* array);
bool diffIndex (void);
void printFromRingBuffer (void);
void clearIndex (void);

#endif /* TERMINAL_H_ */
