/*
 * W25Q32.c
 *
 *  Created on: Nov 20, 2023
 *      Author: edisimo
 */

#include "W25Q32.h"

//will have to rename those functions i guess..

void writeDpi(){
	uint8_t en=WRITE_ENABLE_INSTRUCTION;
	uint8_t se=SECTOR_ERASE_INSTRUCTION;
	uint8_t w=WRITE_INSTRUCTION;


	uint8_t tData[3];

	tData[0]=WRITE_ADDRESS_MSB;
	tData[1]=WRITE_ADDRESS_MID;
	tData[2]=WRITE_ADDRESS_LSB;

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);
	HAL_SPI_Transmit(&hspi1,&en, 1, 1000);

	HAL_SPI_Transmit(&hspi1,&se,1, 1000);

	HAL_SPI_Transmit(&hspi1,&se,1, 1000);




}

void readDpi(){

}


void reset(){
	uint8_t tData[2];
	tData[0]=0x66;
	tData[1]=0x99;

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	HAL_SPI_Transmit(&hspi1, tData, 2, 1000);

    LL_GPIO_SetOutputPin(GPIOA, SPI1_CS_Pin);

    HAL_Delay(100);

}

void id(){
	uint8_t tData = 0x9F;
	uint8_t rData[3];

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);
	HAL_SPI_Transmit(&hspi1, &tData, 1, 1000);
	HAL_SPI_Receive(&hspi1, rData, 3, 3000);

    LL_GPIO_SetOutputPin(GPIOA, SPI1_CS_Pin);
}
