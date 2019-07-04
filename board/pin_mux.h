/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SOPT5_LPUART0ODE_DISABLED 0x00u    /*!<@brief LPUART0 Open Drain Enable: Open drain is disabled on LPUART0. */
#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/*! @name PORTC6 (number 42), J8[1]/U4[4]/U5[25]/UART0_RX_TGTMCU
  @{ */
#define BOARD_INITPINS_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN 6U     /*!<@brief PORTC pin index: 6 */
                                                /* @} */

/*! @name PORTC7 (number 43), J10[1]/U5[24]/U6[1]/UART0_TX_TGTMCU
  @{ */
#define BOARD_INITPINS_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN 7U     /*!<@brief PORTC pin index: 7 */
                                                /* @} */

/*! @name PORTA16 (number 4), J2[1]/Q2[1]/D8/LEDRGB_GREEN
  @{ */
#define BOARD_INITPINS_LED_GREEN_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITPINS_LED_GREEN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_LED_GREEN_PIN 16U    /*!<@brief PORTA pin index: 16 */
                                            /* @} */

/*! @name PORTB2 (number 18), J4[3]/Q3[1]/A2/LEDRGB_BLUE
  @{ */
#define BOARD_INITPINS_LED_BLUE_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITPINS_LED_BLUE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_LED_BLUE_PIN 2U     /*!<@brief PORTB pin index: 2 */
                                           /* @} */

/*! @name PORTC1 (number 37), J2[2]/Q1[1]/D9/LEDRGB_RED
  @{ */
#define BOARD_INITPINS_LED_RED_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITPINS_LED_RED_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_LED_RED_PIN 1U     /*!<@brief PORTC pin index: 1 */
                                          /* @} */
/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
