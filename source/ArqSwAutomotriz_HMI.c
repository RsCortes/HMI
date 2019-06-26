/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ArqSwAutomotriz_HMI.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKW36Z4.h"
#include "fsl_debug_console.h"
#include "ExampleDriver.h"

//CAN

#include "Driver_CAN.h"

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

//Variables
//APLICACION
uint8_t Data_CAN = 0;
uint8_t Data_CAN2 = 0;
//uint8_t i = 0;

int main(void) {

	volatile bool txComplete = false;
	volatile bool rxComplete = false;
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /* Init FSL debug console. */
    BOARD_InitDebugConsole();


    //CAN
	flexcan_config_t flexcanConfig;
			    flexcan_rx_mb_config_t mbConfig;

			    BOARD_InitPins();
			    BOARD_BootClockRUN();
			    BOARD_InitDebugConsole();

				PRINTF("\r\n==FlexCAN loopback example -- Start.==\r\n\r\n");

					/* Init FlexCAN module. */
					/*
					 * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
					 * flexcanConfig.baudRate = 1000000U;
					 * flexcanConfig.maxMbNum = 16;
					 * flexcanConfig.enableLoopBack = false;
					 * flexcanConfig.enableSelfWakeup = false;
					 * flexcanConfig.enableIndividMask = false;
					 * flexcanConfig.enableDoze = false;
					 */

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
					mbConfig.id = FLEXCAN_ID_STD(0x124);
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
					txFrame.id = FLEXCAN_ID_STD(0x125);
				#if (defined(USE_CANFD) && USE_CANFD)
					txFrame.length = DLC;
				#endif

				#if (defined(USE_CANFD) && USE_CANFD)
					txXfer.framefd = &txFrame;
				#endif

					/* Send data through Tx Message Buffer. */
					txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;

    //FIN CAN

    while(1) {
        /*Calling Driver Test*/
    	//TestDriverFunction();
    	/*
    			txFrame.dataWord[0] = 1;
    			txFrame.dataWord[1] = 65;
    			Data_CAN2 = txFrame.dataWord[1];
    			FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    			PRINTF("Data send for CAN: %d\n\r", Data_CAN2);
    	*/
    		if(rxComplete == true)
    			{

    				Data_CAN = rxFrame.dataWord[0];
    				PRINTF("Data Receive for CAN: %d\n\r", Data_CAN);



    				switch (Data_CAN)
    				{
    				 case 0:  /* MODDE OFF */
    							#if (defined(USE_CANFD) && USE_CANFD)

    									txFrame.dataWord[0] = OFF;
    							#endif
    				  break;

    				  case 1:  /* MODDE VELOCITY ONE */

    							#if (defined(USE_CANFD) && USE_CANFD)

    									txFrame.dataWord[0] = LEVEL_ONE;

    							#endif
    				  break;

    				  case 2:  /* MODDE VELOCITY TWO */

    							#if (defined(USE_CANFD) && USE_CANFD)

    									txFrame.dataWord[0] = LEVEL_TWO;

    							#endif
    				  break;

    				  case 3:  /*MODE VELOCITY TREE*/

    							#if (defined(USE_CANFD) && USE_CANFD)

    									txFrame.dataWord[0] = LEVEL_TREE;

    							#endif
    				  break;

    				  case 4:  /*MODDE AUTO*/

    					 				#if (defined(USE_CANFD) && USE_CANFD)

    					 						txFrame.dataWord[0] = MODE_AUTO;

    					 				#endif

    				  break;

    				  default:
    				  break;
    				//}



    			}
    				rxComplete = false;
    				memset(rxFrame.dataWord, 0, ECHO_BUFFER_LENGTH_8BITS);
    				FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
    				FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    				//memset(g_txBuffer, CLEAN, ECHO_BUFFER_LENGTH);
    		}


    	 }
    return 0;
}
