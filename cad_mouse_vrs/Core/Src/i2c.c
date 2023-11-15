/* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file    i2c.c
//  * @brief   This file provides code for the configuration
//  *          of the I2C instances.
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2023 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */
uint8_t *aReceiveBuffer_read;
uint8_t size = 0;
volatile uint8_t ubReceiveIndex = 0;
/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* USER CODE BEGIN I2C1_Init 1 */
   GPIOB->ODR |= (0b11 << 6);
  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x0000020B;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */
  LL_I2C_Enable(I2C1);
  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */
void i2c_master_write(uint8_t *data, uint8_t register_addr, uint8_t slave_addr,uint8_t len)
{
	LL_I2C_HandleTransfer(I2C1, slave_addr, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

	LL_I2C_TransmitData8(I2C1, register_addr);

	while(!LL_I2C_IsActiveFlag_STOP(I2C1))
	{
		for (size_t i = 0; i < len;i++) {
			if(LL_I2C_IsActiveFlag_TXIS(I2C1))
			{
				LL_I2C_TransmitData8(I2C1, data[i]);
			}
		}
	}
	LL_I2C_ClearFlag_STOP(I2C1);
}


void i2c_master_read(uint8_t* data,  uint8_t register_addr, uint8_t slave_addr, uint8_t len)
{

	aReceiveBuffer_read = data;
	size = len;

	LL_I2C_EnableIT_RX(I2C1);

	LL_I2C_HandleTransfer(I2C1, slave_addr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

//	I2C_Start();
	while(!LL_I2C_IsActiveFlag_STOP(I2C1))
	{
		if(LL_I2C_IsActiveFlag_TXIS(I2C1))
		{
			LL_I2C_TransmitData8(I2C1, register_addr);
		}
	}
	LL_I2C_ClearFlag_STOP(I2C1);
	while(LL_I2C_IsActiveFlag_STOP(I2C1)){}

	LL_I2C_HandleTransfer(I2C1, slave_addr, LL_I2C_ADDRSLAVE_7BIT, len, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

	while(!LL_I2C_IsActiveFlag_STOP(I2C1)){};

	//End of transfer
	LL_I2C_ClearFlag_STOP(I2C1);
	LL_I2C_DisableIT_RX(I2C1);
	I2C1->ICR |= (1 << 4);
	LL_I2C_ClearFlag_NACK(I2C1);

	ubReceiveIndex = 0;
}
void I2C1_IRQHandler(void)
{
	if(LL_I2C_IsActiveFlag_RXNE(I2C1))
	{
		/* Call function Master Reception Callback */
		aReceiveBuffer_read[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
		(ubReceiveIndex > 19) ? ubReceiveIndex = 0 : ubReceiveIndex;
	}
	/* Check RXNE flag value in ISR register */

}


/* USER CODE END 1 */
