/*
 * SINI.c
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */
#include "SINI.h"
#include "Driver_TPM.h"

void SINI_vSystemInit ( void )
{
	/*!Add your Inits here*/

	/* Configure tpm params with frequency 24kHZ */
	TPM_Params ();

	/* Initialize TPM module */
	TPM_Initialize();

}
