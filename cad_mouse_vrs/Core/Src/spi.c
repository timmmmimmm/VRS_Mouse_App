/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
uint8_t *receive_buffer;
uint8_t index=0;
/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_RX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);

  /* SPI1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);

  /* SPI1 interrupt Init */
  NVIC_SetPriority(SPI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(SPI1_IRQn);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_EnableNSSPulseMgt(SPI1);
  /* USER CODE BEGIN SPI1_Init 2 */
  SPI1->CR2 |= 1 << 12;
  LL_SPI_Enable(SPI1);
  /* USER CODE END SPI1_Init 2 */

}

/* USER CODE BEGIN 1 */
void spi_master_write_data(uint8_t slave_address, uint8_t register_address, uint8_t length, uint8_t* write_data) {
    // Enable SPI TX interrupts
//    LL_SPI_EnableIT_TX(SPI1);

    // Initialize communication
    LL_SPI_HandleTransfer(SPI1, slave_address, LL_SPI_ADDRSLAVE_7BIT, length+1, LL_SPI_MODE_AUTOEND, LL_SPI_GENERATE_START_WRITE);
    LL_SPI_TransmitData8(SPI1, register_address);
    // Send register address
    while(!LL_SPI_IsActiveFlag_STOP(SPI1)) {
    	for (uint8_t i = 0; i < length; i++) {
    		if(LL_SPI_IsActiveFlag_TXIS(SPI1)) {
    			LL_SPI_TransmitData8(SPI1, write_data[i]);
    		}
		}
    }
    LL_SPI_ClearFlag_STOP(SPI1);

    // End of transfer
//    LL_SPI_DisableIT_TX(SPI1);
}

void spi_master_read_data(uint8_t slave_address, uint8_t register_address, uint8_t length, uint8_t* read_data)
{
	receive_buffer=read_data;
	// Enable It from SPI
	LL_SPI_EnableIT_RX(SPI1);
	// Initialize communication
	LL_SPI_HandleTransfer(SPI1, slave_address, LL_SPI_ADDRSLAVE_7BIT, 1, LL_SPI_MODE_AUTOEND, LL_SPI_GENERATE_START_WRITE);
	// Send register address
	while(!LL_SPI_IsActiveFlag_STOP(SPI1))
	{
		if(LL_SPI_IsActiveFlag_TXIS(SPI1))
		{
			LL_SPI_TransmitData8(SPI1, register_address);
		}
	}
	LL_SPI_ClearFlag_STOP(SPI1);
	while(LL_SPI_IsActiveFlag_STOP(SPI1)){};


	// Receive data from slave device
	LL_SPI_HandleTransfer(SPI1, slave_address, LL_SPI_ADDRSLAVE_7BIT, length, LL_SPI_MODE_AUTOEND, LL_SPI_GENERATE_START_READ);
	index=0;
	while(!LL_SPI_IsActiveFlag_STOP(SPI1)){};

	//End of transfer
	LL_SPI_DisableIT_RX(SPI1);
	LL_SPI_ClearFlag_STOP(SPI1);
	LL_SPI_ClearFlag_NACK(SPI1);
}

void SPI_INTERRUPT_HANDLER(){
	if (LL_SPI_IsActiveFlag_RXNE(SPI1)){
		receive_buffer[index++]=LL_SPI_ReceiveData8(SPI1);
		if (index>=16){
			index=0;
		}
	}
}
/* USER CODE END 1 */
