/*
 * UART.c
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */

#include "UART.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"

lpuart_config_t stUartConfig;

static volatile uint8_t UART__au8RxData;
static volatile uint8_t *UART__au8TxBuffer;
static volatile uint8_t UART__u8DataCounter;

uint8_t g_tipString[] =
    "Lpuart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";


//! UART driver status register
uint32_t UART_u32DriverStatus = 0;

void UART_vInit ( void )
{
	CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

	PORT_SetPinMux(PORTC, 6, kPORT_MuxAlt4);            /* PORTC6 (pin 42) is configured as LPUART0_RX */
	PORT_SetPinMux(PORTC, 7, kPORT_MuxAlt4);            /* PORTC7 (pin 43) is configured as LPUART0_TX */

	SIM->SOPT5 = ((SIM->SOPT5 &
	    (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK))) /* Mask bits to zero which are setting */
	      | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* LPUART0 Transmit Data Source Select: LPUART0_TX pin */
	      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
	    );

    CLOCK_SetLpuart0Clock(0x1U);
	LPUART_GetDefaultConfig(&stUartConfig);
	stUartConfig.baudRate_Bps = 115200;
	stUartConfig.enableTx = true;
	stUartConfig.enableRx = true;

	LPUART_Init(LPUART0, &stUartConfig, CLOCK_GetFreq(kCLOCK_McgFllClk));

	/* Enable RX interrupt. */
	LPUART_EnableInterrupts(LPUART0, kLPUART_RxDataRegFullInterruptEnable);
	EnableIRQ(LPUART0_LPUART1_IRQn);

}


void UART_vTxData ( uint8_t *pu8Data, uint8_t u8MsgLenght )
{
	UART__au8TxBuffer = pu8Data;
	UART__u8DataCounter = u8MsgLenght;
	UART_CLEAR_STATUS(UART0_TX);
	LPUART0->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_TIE_MASK;
}

uint8_t UART_u8RxData ( void )
{
	UART_CLEAR_STATUS(UART0_RX);
	return UART__au8RxData;
}

void LPUART0_LPUART1_IRQHandler (void )
{
	if( LPUART0->STAT & LPUART_STAT_TDRE_MASK && LPUART0->CTRL & LPUART_CTRL_TE_MASK )
	{
		if( UART__u8DataCounter -- )
		{
			LPUART0->DATA = *UART__au8TxBuffer++;
		}
		else
		{
			LPUART0->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_TIE_MASK)	;
			UART_SET_STATUS(UART0_TX);
		}
	}
	else
	{
		/*Nothing to do*/
	}

	if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART0))
	{
		UART__au8RxData =  LPUART_ReadByte(LPUART0);
		UART_SET_STATUS(UART0_RX);
	}
	else
	{
		/*Nothing to do*/
	}
}

uint8_t UART_u8GetRxStatus ( void )
{
	return UART_CHECK_STATUS(UART0_RX);
}

void UART_vAppTest ( void )
{
	uint8_t u8RxData = 0;
	if( UART_u8GetRxStatus() )
	{
		u8RxData = UART_u8RxData();
		UART_vTxData(&u8RxData, 1);
	}

}
