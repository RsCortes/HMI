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

#define MIDDLELAYER_BUFFER_LENGTH (7)
#define MAX_APP_BUFFERS (5)
#define CAN_FD_BUFFER_LENGTH (8)

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	uint8_t u8_BufferRcvEmissions [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvEmgcyStop [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvRPM [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvFan [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvTemperature [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvThrottle [MIDDLELAYER_BUFFER_LENGTH];
}st_CANReceiveBuffers;

typedef struct
{
	uint8_t u8_BufferSendEmissions [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendEmgcyStop [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendRPM [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendFan [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendTemperature [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendThrottle [MIDDLELAYER_BUFFER_LENGTH];
}st_CANSendBuffers;

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

static st_CANReceiveBuffers RcvApplicationBuffers;
static st_CANSendBuffers SendApplicationBuffers;

static st_CANReceiveBuffers *pst_ReceiveAppBuffers = &RcvApplicationBuffers;
static st_CANSendBuffers *pst_SendAppBuffers = &SendApplicationBuffers;

static uint8_t u8_CANIncomingData[3];

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
//	pstStruct->u8_BufferRcvEmgcyStop[0]

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
void vFUN_FlexCANConfig ( void )
{
	flexcan_config_t flexcanConfig;
	flexcan_rx_mb_config_t mbConfig;

		FLEXCAN_GetDefaultConfig(&flexcanConfig);
	#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
		flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
	#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */
		flexcanConfig.enableLoopBack = true;
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
}

/*!
 * @brief FlexCAN send data function
 */
void vFUN_FlexCANSendNonBlocking ( void )
{
	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendEmissions[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);

	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendEmgcyStop[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);

	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendRPM[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);

	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendFan[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);

	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendTemperature[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);

	memcpy(&txFrame.dataWord[0], &pst_SendAppBuffers->u8_BufferSendThrottle[0], MIDDLELAYER_BUFFER_LENGTH);
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
	memset(&txFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
}

/*!
 * @brief FlexCAN receive data function
 */
void vFUN_FlexCANReceiveNonBlocking ( void )
{
	if(rxComplete && (rxFrame.dataWord[0] == 65))
	{
		memcpy(&u8_CANIncomingData[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);

		switch( u8_CANIncomingData[2] )
		{
			case 0:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvEmissions[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;

			case 1:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvEmgcyStop[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;

			case 2:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvRPM[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;

			case 3:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvFan[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;

			case 4:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvTemperature[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;

			case 5:
				memcpy(&pst_ReceiveAppBuffers->u8_BufferRcvThrottle[0], &rxFrame.dataWord[0], MIDDLELAYER_BUFFER_LENGTH);
				memset(&rxFrame.dataWord[0], 0, CAN_FD_BUFFER_LENGTH);
				break;
		}

		rxComplete = false;
		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
	}
	else
	{
		/* Nothing to do here */
	}
}

void vFUN_Start2Listen_FlexCAN ( void )
{
	FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
