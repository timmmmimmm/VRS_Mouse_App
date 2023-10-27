/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t bufferUSART2dma[DMA_USART2_BUFFER_SIZE];

/* Declaration and initialization of callback function */
static void (* USART2_ProcessData)(uint8_t data) = 0;

/* Register callback */
void USART2_RegisterCallback(void *callback)
{
	if(callback != 0)
	{
		USART2_ProcessData = callback;
	}
}

/* Space for global variables, if you need them */

	// type global variables here
uint16_t buf_read_pos = 0;
/* USER CODE END 0 */

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
  	  NVIC_EnableIRQ(DMA1_Channel6_IRQn);
    /* DMA1_Channel7_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel7_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel7_IRQn);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA15   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = VCP_TX_Pin|VCP_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_Init 1 */
  /* USART2_RX Init */
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PRIORITY_MEDIUM);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MDATAALIGN_BYTE);

    LL_DMA_ConfigAddresses(	DMA1, LL_DMA_CHANNEL_6,
  						 	LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_RECEIVE),
  							(uint32_t)bufferUSART2dma,
  							LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6));

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, DMA_USART2_BUFFER_SIZE);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
    LL_USART_EnableDMAReq_RX(USART2);


    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_6);
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_6);


    /* USART2_TX Init */
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PRIORITY_MEDIUM);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MODE_NORMAL);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MDATAALIGN_BYTE);

    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_7, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT));
    LL_USART_EnableDMAReq_TX(USART2);

    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);

    /* USART2 interrupt Init */
    NVIC_SetPriority(USART2_IRQn, 0);
    NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART2);
  LL_USART_ConfigAsyncMode(USART2);
  /* USER CODE BEGIN USART2_Init 2 */
  LL_USART_EnableIT_IDLE(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE END USART2_Init 2 */

}

/* USER CODE BEGIN 1 */

void USART2_PutBuffer(uint8_t *buffer, uint8_t length)
{
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_7, (uint32_t)buffer);

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, length);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
}


/*
 *	Function processing data received via USART2 with DMA and stored in bufferUSART2dma.
 *	Forwards data to callback function.
 *	Keeps track of pointer pointing to Rx memory buffer and resets the pointer if overflow is possible in next Rx.
 *	Refer to reference manual - "normal memory mode" and "increment memory mode".
 */
void USART2_CheckDmaReception(void)
{
	uint16_t pos = DMA_USART2_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);

	if (pos >= DMA_USART2_BUFFER_SIZE)
	{
		// set the DMA address pointer back to the beginning of the buffer
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_6, (uint32_t)bufferUSART2dma);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, DMA_USART2_BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);

		// process all data until the end of the buffer
		while(buf_read_pos < DMA_USART2_BUFFER_SIZE)
		{
			if(USART2_ProcessData != 0)
			{
				USART2_ProcessData(bufferUSART2dma[buf_read_pos]);
			}
			buf_read_pos++;
		}

		buf_read_pos = 0;
	}
	else
	{
		// process new data
		while(buf_read_pos < pos)
		{
			if(USART2_ProcessData != 0)
			{
				USART2_ProcessData(bufferUSART2dma[buf_read_pos]);
			}
			buf_read_pos++;
		}
	}
}



/* USER CODE END 1 */
