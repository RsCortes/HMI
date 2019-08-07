/*
 * SINI.c
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */
#include "SINI.h"
#include <CAN_Module/CAN.h>
#include <GPIO/GPIO_Init.h>
#include "Driver_TPM.h"
#include "UART.h"
#include "clock_config.h"

void SINI_vSystemInit ( void )
{
	/*!Add your Inits here*/
	vFUN_EnableCANGpio();
	vFUN_FlexCANConfig();

	vFUN_EnableTPMGpio();
	vFUN_Start2Listen_FlexCAN();
	UART_vInit();
	TPM_Params ();
	TPM_Initialize();
    BOARD_BootClockRUN();
}
