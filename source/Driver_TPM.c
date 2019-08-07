#include "Driver_TPM.h"


#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

#include "pin_mux.h"




/*******************************************************************************
 * Variables TPM
 ******************************************************************************/
volatile bool brightnessUp = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 0U;
volatile uint8_t getCharValue = 0U;
tpm_config_t tpmInfo;
tpm_chnl_pwm_signal_param_t tpmParam;
uint16_t cnv3;


/*******************************************************************************
 * Functions TPM
 ******************************************************************************/
void TPM_InitPins (void)
{
BOARD_InitPins();



}

void TPM_Params (void)
{
/* Select the clock source for the TPM i as MCGFLLCLK */
CLOCK_SetTpmClock(1U);

tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
tpmParam.level = TPM_LED_ON_LEVEL;
tpmParam.dutyCyclePercent = updatedDutycycle;


}

void TPM_Initialize(void)
{
TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
TPM_GetDefaultConfig(&tpmInfo);
TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_EdgeAlignedPwm,24000U, TPM_SOURCE_CLOCK);
BOARD_TPM_BASEADDR->SC = TPM_SC_PS(7);
BOARD_TPM_BASEADDR->MOD = 6250;
TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
cnv3 = 300;
BOARD_TPM_BASEADDR->CONTROLS[BOARD_TPM_CHANNEL].CnV = cnv3;
TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
}

/*
void Servo_Move(uint16_t cnv2)
{
BOARD_TPM_BASEADDR->CONTROLS[BOARD_TPM_CHANNEL].CnV = cnv2;
}
*/




























