#ifndef DRIVER_TPM_H_
#define DRIVER_TPM_H_

/*******************************************************************************
 * Definitions TPM
 ******************************************************************************/
#define BOARD_TPM_BASEADDR TPM1
#define BOARD_TPM_CHANNEL 0U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM1_IRQn
#define TPM_LED_HANDLER TPM1_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgFllClk)

/*******************************************************************************
 * Functions TPM
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

void TPM_InitPins (void);
void TPM_Initialize(void);
void TPM_Params (void);
/*void Servo_Move(uint16_t cnv);
*/

#if defined(__cplusplus)
}
#endif // __cplusplus


#endif /* DRIVER_TPM */


