/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file app_gpio.c
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "app_gpio.h"

/** @addtogroup
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Configures LED GPIO.
 * @param Led Specifies the Led to be configured.
 *   This parameter can be GPIO_PIN_0~GPIO_PIN_13.
 */
void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_InitType GPIO_InitStructure;

	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

	/* Enable the GPIO Clock */
	if (GPIOx == GPIOA) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	} else if (GPIOx == GPIOB) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
	} else {
		return;
	}

	/* Configure the GPIO pin as output push-pull */
	if (Pin <= GPIO_PIN_ALL) {
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin = Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
	}
}
/**
 * @brief  Turns selected Led on.
 * @param GPIOx x can be A to B to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_13.
 */
void LedOn(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_SetBits(GPIOx, Pin);
}

/**
 * @brief  Turns selected Led Off.
 * @param GPIOx x can be A to B to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_13.
 */
void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_ResetBits(GPIOx, Pin);
}

/**
 * @brief  Toggles the selected Led.
 * @param GPIOx x can be A to B to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_13.
 */
void LedBlink(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_TogglePin(GPIOx, Pin);
}

void key_init(void)
{
	GPIO_InitType GPIO_InitStructure;
	/* Enable the GPIO Clock */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);

	/*Configure the GPIO pin as input floating */
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin          = KEY1_INPUT_PIN;
	GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
	GPIO_InitPeripheral(KEY1_INPUT_PORT, &GPIO_InitStructure);

	/* Enable the GPIO Clock */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);

	/*Configure the GPIO pin as input floating*/
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin          = KEY2_INPUT_PIN;
	GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
	GPIO_InitPeripheral(KEY2_INPUT_PORT, &GPIO_InitStructure);
}
void btn_to_start_up(void)
{
	key_init();
	LedInit(LED1_PORT, LED1_PIN);
	LedInit(LED2_PORT, LED2_PIN);
	while(1) {
		if((GPIO_ReadInputDataBit(KEY1_INPUT_PORT, KEY1_INPUT_PIN) == 0) || (GPIO_ReadInputDataBit(KEY2_INPUT_PORT, KEY2_INPUT_PIN) == 0)) {
			break;
		}
		delay_n_ms(231);
		LedBlink(LED1_PORT, LED1_PIN);
		if((GPIO_ReadInputDataBit(KEY1_INPUT_PORT, KEY1_INPUT_PIN) == 0) || (GPIO_ReadInputDataBit(KEY2_INPUT_PORT, KEY2_INPUT_PIN) == 0)) {
			break;
		}
		delay_n_ms(231);
		LedBlink(LED2_PORT, LED2_PIN);
	}
	LedOff(LED1_PORT, LED1_PIN);
	LedOff(LED2_PORT, LED2_PIN);
}



/**
 * @}
 */
