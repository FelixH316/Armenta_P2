/*** INCLUDED LIBRARIES ***/
// MAIN LIBRARIES
#include <stdbool.h>
#include <stdint.h>

#include "clock_config.h"
#include "terminal.h"
#include "ds1307.h"
#include "board.h"


// UART LIBRARIES
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#define TIME_PCKG 0
#define DATE_PCKG 1

typedef enum {mainMenu, configTime, configDate, readTime, readDate} menu_state_t;

int main(void) {
	/*** PERIPHERALS INIT ***/
	UART0_Init(UART0_PORT, UART0_TX, UART0_RX);
	I2C0_Init(I2C0_PORT, I2C0_SDA, I2C0_SCL);


	/*** SYSTEM CLOCK & BUS CLOCK CONFIG ***/
	BOARD_BootClockRUN();	// System Clock 120 MHz & Bus Clock 60 MHz


	/*** PERIPHERALS CONFIG ***/
	UART0_Config(UART_BAUDRATE);
	I2C0_Config(I2C_BAUDRATE);
	BOARD_InitDebugConsole();


	/*** INIT SYSTEM VARIABLES ***/
	setUartIRQState(menuInput);
	const uint8_t menuString[] = "\n\r\tOpciones:\n\r\t1) Configurar Hora\n\r\t2) Configurar Fecha\n\r\t3) Leer Hora\n\r\t4) Leer Fecha\n\r\t->";
	const uint8_t strTimeInst[] = "\n\n\r\tIntroducir Hora con el fomrmato hh:mm:ss\n\r\t-> ";
	const uint8_t strTimeDone[] = "\n\r\tSe ha configurado la hora";
	const uint8_t strDateInst[] = "\n\n\r\tIntroducir la fecha actual con el formato dd/mm/aa:\n\r\t-> ";
	const uint8_t strDateDone[] = "\n\r\tSe ha configurado la fecha";
	const uint8_t string3[] = "\n\n\r\tLa hora actual es:\n\r\t-> ";
	const uint8_t string4[] = "\n\n\r\tLa fecha actual es:\n\r\t-> ";

	bool newMenu = 1;
	menu_state_t menuCurrentState;
	bool selectEnable = false;
	bool configLoopFlag = true;

	uint8_t timeArray[8] = { 0 };
	uint8_t dateArray[8] = { 0 };
	status_t status_i2c;
	//uint8_t I2C_master_txBuff[DS1307_DATA_LENGTH] = { 0x80, 0x22, 0x11, 0x00, 0x12, 0x06, 0x21 };

	while (1) {
		if ((newMenu) && (getUartIRQState() == menuInput)) {
			UART_WriteBlocking(UART0, menuString, sizeof(menuString) / sizeof(menuString[0]));
			newMenu = false;
		}

		if (optionSelected (1)) {
			menuCurrentState = configTime;
			setUartIRQState (timeInput);
			cleanMenuInputs(&selectEnable);
		} else if (optionSelected (2)) {
			menuCurrentState = configDate;
			setUartIRQState (dateInput);
			cleanMenuInputs(&selectEnable);
		} else if (optionSelected (3)) {
			menuCurrentState = readTime;
			setUartIRQState (escInput);
			cleanMenuInputs(&selectEnable);
		} else if (optionSelected (4)) {
			menuCurrentState = readDate;
			setUartIRQState(escInput);
			cleanMenuInputs(&selectEnable);
		}
		if ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (diffIndex())) {
			printFromRingBuffer();
		}

		if (selectEnable) {
			clearIndex();
			switch (menuCurrentState) {
			case configTime:
				UART_WriteBlocking(UART0, strTimeInst, sizeof(strTimeInst) / sizeof(strTimeInst[0]));
				while (configLoopFlag) {
					if (getEnterFlag()) {
						saveArray (timeArray);
						configLoopFlag = false;
						setEnterFlag (false);
					}
					/* Send data only when UART TX register is empty and ring buffer has data to send out. */
					else if ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (diffIndex())) {
						printFromRingBuffer();
					}
				}
				status_i2c = sentPackageI2C(TIME_PCKG, timeArray);
				UART_WriteBlocking(UART0, strTimeDone, sizeof(strTimeDone) / sizeof(strTimeDone[0]));
				clearIndex ();
				setUartIRQState (escInput);
				while (!getEscFlag()) {
				}
				setEscFlag(false);
				configLoopFlag = true;
				menuCurrentState = mainMenu;
				setUartIRQState (menuInput);
				break;
			case configDate:
				UART_WriteBlocking(UART0, strDateInst, sizeof(strDateInst) / sizeof(strDateInst[0]));
				while(configLoopFlag){
					if (getEnterFlag()) {
						saveArray (dateArray);
						configLoopFlag = false;
						setEnterFlag (false);
					}
					/* Send data only when UART TX register is empty and ring buffer has data to send out. */
					else if ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (diffIndex())) {
						printFromRingBuffer();
					}
				}
				status_i2c = sentPackageI2C(DATE_PCKG, dateArray);
				UART_WriteBlocking(UART0, strDateDone, sizeof(strDateDone) / sizeof(strDateDone[0]));
				clearIndex ();
				setUartIRQState (escInput);
				while (!getEscFlag()) {
				}
				setEscFlag(false);
				configLoopFlag = true;
				menuCurrentState = mainMenu;
				setUartIRQState (menuInput);
				break;
			case readTime:
				UART_WriteBlocking(UART0, string3, sizeof(string3) / sizeof(string3[0]));
				break;
			case readDate:
				UART_WriteBlocking(UART0, string4, sizeof(string4) / sizeof(string4[0]));
				break;
			default:
				UART_WriteByte(UART0, 'D');
			}
			UART_WriteByte(UART0, '\n');
			selectEnable = false;
			newMenu = true;
		}
	}
}
