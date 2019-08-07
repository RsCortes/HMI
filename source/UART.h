/*
 * UART.h
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */

#ifndef UART_H_
#define UART_H_

#include "MKW36Z4.h"
#include "fsl_clock.h"

extern void UART_vInit ( void );
extern void UART_vAppTest ( void );
extern uint8_t UART_u8RxData ( void );
extern void UART_vTxData ( uint8_t *pu8Data, uint8_t u8MsgLenght );
extern uint8_t UART_u8GetRxStatus ( void );

typedef enum
{
	UART0_RX = 0,
	UART0_TX
}UART_tenDevicesStatus;

#define UART_CHECK_STATUS(X)	(UART_u32DriverStatus&(1<<X))
#define UART_SET_STATUS(X)		(UART_u32DriverStatus |= (1<<X))
#define UART_CLEAR_STATUS(X) 	(UART_u32DriverStatus &=~ (1<<X))

#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX  0x00u   /*!< LPUART0 Transmit Data Source Select: LPUART0_TX pin */

#define UART_BAUDRATE					115200
#endif /* UART_H_ */
