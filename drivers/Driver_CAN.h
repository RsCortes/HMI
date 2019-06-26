/*
 * Driver_CAN.h
 *
 *  Created on: 25/06/2019
 *      Author: Marcelo
 */

#ifndef DRIVER_CAN_H_
#define DRIVER_CAN_H_

#include "stdint.h"

#define DWORD_IN_MB (8)
#define DLC (13)
#define BYTES_IN_MB kFLEXCAN_32BperMB
#define RX_MESSAGE_BUFFER_NUM (8)
#define TX_MESSAGE_BUFFER_NUM (9)

#define ECHO_BUFFER_LENGTH 8
#define ECHO_BUFFER_LENGTH_8BITS 8

#define OFF 0
#define LEVEL_ONE 1
#define LEVEL_TWO 2
#define LEVEL_TREE 3
#define MODE_AUTO 4

/*******************************************************************************
 * Definitions for flexcan_transfer
 ******************************************************************************/
#define EXAMPLE_CAN CAN0
#define EXAMPLE_CAN_CLKSRC kCLOCK_BusClk
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define USE_CANFD (1)

#if defined(__cplusplus)
extern "C" {
#endif // __cpluplus

/* flexcan callback */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);

#if defined(__cplusplus)
}
#endif // __cpluplus


#endif /* DRIVER_CAN_H_ */
