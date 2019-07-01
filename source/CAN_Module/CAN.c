/*
 * CAN.c
 *
 *  Created on: 01/07/2019
 *      Author: Rodrigo Serrano
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "CAN.h"
#include "stdbool.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#define EXAMPLE_CAN CAN0
#define EXAMPLE_CAN_CLKSRC kCLOCK_BusClk
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define USE_CANFD (1)
/*
 *    DWORD_IN_MB    DLC    BYTES_IN_MB             Maximum MBs
 *    2              8      kFLEXCAN_8BperMB        32
 *    4              10     kFLEXCAN_16BperMB       21
 *    8              13     kFLEXCAN_32BperMB       12
 *    16             15     kFLEXCAN_64BperMB       7
 *
 * Dword in each message buffer, Length of data in bytes, Payload size must align,
 * and the Message Buffers are limited corresponding to each payload configuration:
 */
#define DWORD_IN_MB (8)
#define DLC (13)
#define BYTES_IN_MB kFLEXCAN_32BperMB
#define RX_MESSAGE_BUFFER_NUM (8)
#define TX_MESSAGE_BUFFER_NUM (9)

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

/* flexcan callback */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////

volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_mb_transfer_t txXfer;
flexcan_mb_transfer_t rxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t txFrame;
flexcan_fd_frame_t rxFrame;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief FlexCAN configuration function
 */
bool bFUN_FlexCANConfig(void)
{
	flexcan_config_t flexcanConfig;
	flexcan_rx_mb_config_t mbConfig;

		FLEXCAN_GetDefaultConfig(&flexcanConfig);
	#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
		flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
	#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */
		flexcanConfig.enableLoopBack = false;
	#if (defined(USE_CANFD) && USE_CANFD)
		FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, false);
	#endif

		/* Setup Rx Message Buffer. */
		mbConfig.format = kFLEXCAN_FrameFormatStandard;
		mbConfig.type = kFLEXCAN_FrameTypeData;
		mbConfig.id = FLEXCAN_ID_STD(0x123);
	#if (defined(USE_CANFD) && USE_CANFD)
		FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
	#endif

		/* Setup Tx Message Buffer. */
	#if (defined(USE_CANFD) && USE_CANFD)
		FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
	#endif

		/* Create FlexCAN handle structure and set call back function. */
		FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

		/* Start receive data through Rx Message Buffer. */
		rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
	#if (defined(USE_CANFD) && USE_CANFD)
		rxXfer.framefd = &rxFrame;
		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
	#endif

		/* Prepare Tx Frame for sending. */
		txFrame.format = kFLEXCAN_FrameFormatStandard;
		txFrame.type = kFLEXCAN_FrameTypeData;
		txFrame.id = FLEXCAN_ID_STD(0x124);
	#if (defined(USE_CANFD) && USE_CANFD)
		txFrame.length = DLC;
	#endif

	#if (defined(USE_CANFD) && USE_CANFD)
		txXfer.framefd = &txFrame;
	#endif

		/* Send data through Tx Message Buffer. */
		txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;

	return true;
}

/*!
 * @brief FlexCAN send data function
 */
//bool bFUN_FlexCANSendNonBlocking(void)
//{
//	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
//	return true;
//}

/*!
 * @brief FlexCAN receive data function
 */
//bool bFUN_FlexCANReceiveNonBlocking(void)
//{
//	FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
//	return true;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
