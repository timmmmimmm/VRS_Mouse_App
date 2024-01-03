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
I2C_HandleTypeDef hi2c1;
/* USER CODE END 0 */

/* I2C1 init function */    ///////////////////////   LL FN  OLD   //////////////////////////


/////////////////////////////////            HAL FN NEW   ///////////////////////////////////

void MX_I2C1_Init(void)
{


	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x0000020B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}


/* USER CODE BEGIN 1 */

///////////////////////////         NEW WRITE FN      ////////////////////////////////////////////////////

HAL_StatusTypeDef i2c_master_write(uint8_t slave_addr, uint8_t *data, uint8_t len)
{
	return HAL_I2C_Master_Transmit_DMA(&hi2c1, slave_addr, data, len);
}



///////////////////////////         NEW READ FN      ////////////////////////////////////////////////////

HAL_StatusTypeDef i2c_master_read(uint8_t slave_addr, uint8_t *data, uint8_t len)
{
	return HAL_I2C_Master_Receive_DMA(&hi2c1, slave_addr, data, len);
}


/* USER CODE END 1 */
