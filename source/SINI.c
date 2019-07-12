/*
 * SINI.c
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */
#include "SINI.h"
#include <CAN_Module/CAN.h>
#include <GPIO/GPIO_Init.h>

void SINI_vSystemInit ( void )
{
	/*!Add your Inits here*/
	vFUN_EnableCANGpio();
	vFUN_FlexCANConfig();

	vFUN_EnableTPMGpio();

	vFUN_Start2Listen_FlexCAN();
}
