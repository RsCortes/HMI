/*
 * SysTasks.c
 *
 *  Created on: Jul 2, 2019
 *      Author: pmg
 */

#include "SysTasks.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKW36Z4.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "CAM.h"
#include "SINI.h"

static void CAM10ms(void * Param);
static void CAM20ms(void * Param);
static void CAM100ms(void * Param);
static void CAM1000ms(void * Param);

/**
 * @name:   CreateSystemTasks
 * @brief:  Function to create the cyclic tasks
 * @param:  None
 * @return: None
 */
void CreateSystemTasks ( void )
{
	xTaskCreate(CAM10ms, "CAM10", configMINIMAL_STACK_SIZE + 128, NULL, 2, NULL);

	xTaskCreate(CAM20ms, "CAM20", configMINIMAL_STACK_SIZE + 128, NULL, 2, NULL);

	xTaskCreate(CAM100ms, "CAM100", configMINIMAL_STACK_SIZE + 128, NULL, 2, NULL);

	xTaskCreate(CAM1000ms, "CAM1000", configMINIMAL_STACK_SIZE + 128, NULL, 2, NULL);
}

static void CAM10ms(void * Param)
{
    gpio_pin_config_t PinConfig =
    {
        kGPIO_DigitalOutput, 0,
    };
    TickType_t Delay;
    TickType_t CurrentTicks;

    /* TaskDelayUntil is used to set an absolute time time 	*/
    /* This task will use a 10 ms delay					*/
    Delay = 10 / portTICK_PERIOD_MS;

    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &PinConfig);

	while(1)
	{
		//GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1 << BOARD_LED_GREEN_GPIO_PIN);
		CAM_Containter10ms();
		/* Make sure we recalculate to make it more precise 				*/
		/* If timing accounting for task process is required				*/
		/* We could use xTaskGetTickCount before task processing start		*/
		/* again at task processing end and get that delta to subtract		*/
		/* from Delay														*/
		CurrentTicks = xTaskGetTickCount();

		vTaskDelayUntil(&CurrentTicks,Delay);
	}
}


static void CAM20ms(void * Param)
{
    gpio_pin_config_t PinConfig =
    {
        kGPIO_DigitalOutput, 0,
    };
    TickType_t Delay;
    TickType_t CurrentTicks;

    /* TaskDelayUntil is used to set an absolute time time 	*/
    /* This task will use a 20 ms delay					*/
    Delay = 20 / portTICK_PERIOD_MS;

    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &PinConfig);

	while(1)
	{
		//GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1 << BOARD_LED_GREEN_GPIO_PIN);
		CAM_Containter20ms();
		/* Make sure we recalculate to make it more precise 				*/
		/* If timing accounting for task process is required				*/
		/* We could use xTaskGetTickCount before task processing start		*/
		/* again at task processing end and get that delta to subtract		*/
		/* from Delay														*/
		CurrentTicks = xTaskGetTickCount();

		vTaskDelayUntil(&CurrentTicks,Delay);
	}
}

static void CAM100ms(void * Param)
{
    gpio_pin_config_t PinConfig =
    {
        kGPIO_DigitalOutput, 0,
    };
    TickType_t Delay;
    TickType_t CurrentTicks;

    /* TaskDelayUntil is used to set an absolute time time 	*/
    /* This task will use a 100 ms delay					*/
    Delay = 100 / portTICK_PERIOD_MS;

    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &PinConfig);

	while(1)
	{
		GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1 << BOARD_LED_GREEN_GPIO_PIN);
		CAM_Containter100ms();
		/* Make sure we recalculate to make it more precise 				*/
		/* If timing accounting for task process is required				*/
		/* We could use xTaskGetTickCount before task processing start		*/
		/* again at task processing end and get that delta to subtract		*/
		/* from Delay														*/
		CurrentTicks = xTaskGetTickCount();

		vTaskDelayUntil(&CurrentTicks,Delay);
	}
}

static void CAM1000ms(void * Param)
{
    gpio_pin_config_t PinConfig =
    {
        kGPIO_DigitalOutput, 0,
    };
    TickType_t Delay;
    TickType_t CurrentTicks;

    /* TaskDelayUntil is used to set an absolute time time 	*/
    /* This task will use a 1000 ms delay					*/
    Delay = 1000 / portTICK_PERIOD_MS;

    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &PinConfig);

	while(1)
	{
		//GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1 << BOARD_LED_GREEN_GPIO_PIN);
		CAM_Containter1000ms();
		/* Make sure we recalculate to make it more precise 				*/
		/* If timing accounting for task process is required				*/
		/* We could use xTaskGetTickCount before task processing start		*/
		/* again at task processing end and get that delta to subtract		*/
		/* from Delay														*/
		CurrentTicks = xTaskGetTickCount();

		vTaskDelayUntil(&CurrentTicks,Delay);
	}
}

void InitSystem ( void )
{
	SINI_vSystemInit();
}
