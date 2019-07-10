/*
 * CAM.c
 *
 *  Created on: Jul 2, 2019
 *      Author: pmg
 */

#include "CAM.h"
#include "UART.h"
/**
 * @name:   CAM_Containter10ms
 * @brief:  This function will contain the app that runs at 10 ms
 * @param:  None
 * @return: None
 */
void CAM_Containter10ms( void )
{
	__asm("nop");
}

/**
 * @name:   CAM_Containter20ms
 * @brief:  This function will contain the app that runs at 20 ms
 * @param:  None
 * @return: None
 */
void CAM_Containter20ms( void )
{
	__asm("nop");
}

/**
 * @name:   CAM_Containter100ms
 * @brief:  This function will contain the app that runs at 100 ms
 * @param:  None
 * @return: None
 */
void CAM_Containter100ms( void )
{
	__asm("nop");
	UART_vAppTest();
}
/**
 * @name:   CAM_Containter1000ms
 * @brief:  This function will contain the app that runs at 100 ms
 * @param:  None
 * @return: None
 */
void CAM_Containter1000ms( void )
{
	__asm("nop");
}
