/*
 * W25Q32.c
 *
 *  Created on: Nov 20, 2023
 *      Author: edisimo
 */

#include "W25Q32.h"

//will have to rename those functions i guess..
//make the functions in SPI, then just use them as callback functions in here ....
//add cs low, high as a callback function

void W25Q32_REGISTERCALLBACKS(void *receive_callback, void *send_callback){
	if (receive_callback!=0){
			W25Q32_RECEIVE=receive_callback;
		}
	if (send_callback!=0){
			W25Q32_SEND=send_callback;
		}
}

void erase_sector(uint16_t numsector){
	uint8_t tData[4];
	uint32_t memAddr = numsector*256*16; //16 pages, 256 bytes
	uint8_t se=SECTOR_ERASE_INSTRUCTION;

	write_enable();

	tData[0]=se;
	tData[1]=(memAddr >> 16)&0xFF; //MSB
	tData[2]=(memAddr >> 8)&0xFF; //MID
	tData[3]=(memAddr)&0xFF; //LSB

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	W25Q32_SEND(tData, 4);

	LL_GPIO_SetOutputPin(GPIOA,SPI1_CS_Pin);
	HAL_Delay(450);

	write_disable();
}

uint32_t bytes_to_write(uint32_t size,uint16_t offset){
	if ((size+offset)<256) return size;
	else return 256-offset;
}


void write(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data){
	uint8_t en = WRITE_INSTRUCTION;
	uint8_t tData[266];
	uint32_t startPage = page;
	uint32_t endPage = startPage + ((size+offset-1)/256);
	uint32_t numPages = endPage-startPage+1;

	uint16_t startSector = startPage/16;
	uint16_t endSector = endPage/16;
	uint16_t numSectors = endSector - startSector + 1;

	for(uint16_t i=0;i<numSectors; i++){
		erase_sector(startSector++);
	}

	uint32_t dataPosition = 0;

	for(uint32_t i=0;i<numPages; i++){
		uint32_t memAddr = (startPage*256)+offset;
		uint16_t bytesremaining = bytes_to_write(size, offset);

		write_enable();

		tData[0]=en;
		tData[1]=(memAddr >> 16)&0xFFU; //MSB
		tData[2]=(memAddr >> 8)&0xFFU; //MID
		tData[3]=(memAddr)&0xFFU; //LSB
		uint32_t index = 4;

		uint16_t bytestosend = bytesremaining + index;

		for (uint16_t i=0; i < bytesremaining; i++){
			tData[index++]=data[i+dataPosition];
		}
		LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);
		W25Q32_SEND(tData, bytestosend);
		LL_GPIO_SetOutputPin(GPIOA,SPI1_CS_Pin);

		startPage++;
		offset=0;
		size = size - bytesremaining;
		dataPosition = dataPosition + bytesremaining;

		HAL_Delay(5);
		write_disable();
	}
}

void write_enable(){
	uint8_t en=WRITE_ENABLE_INSTRUCTION;

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	W25Q32_SEND(&en, 1);
	LL_GPIO_SetOutputPin(GPIOA,SPI1_CS_Pin);
	HAL_Delay(10);
}

void write_disable(){
	uint8_t en=WRITE_DISABLE_INSTRUCTION;

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	W25Q32_SEND(&en, 1);
	LL_GPIO_SetOutputPin(GPIOA,SPI1_CS_Pin);
	HAL_Delay(10);
}


void readDpi(uint32_t startPage,uint8_t offset, uint8_t size, uint8_t * rData){
	uint8_t en = READ_INSTRUCTION;
	uint32_t address = (startPage*256)+offset;

	uint8_t tData[4];
	tData[0]=en;
	tData[1]=(address >> 16)&0xFFU; //MSB
	tData[2]=(address >> 8)&0xFFU; //MID
	tData[3]=(address)&0xFFU; //LSB

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	W25Q32_SEND(tData, 4);
	HAL_Delay(10);
	W25Q32_RECEIVE(rData, size);


	LL_GPIO_SetOutputPin(GPIOA, SPI1_CS_Pin);

	HAL_Delay(100);
}


void reset(){
	uint8_t tData[2];
	tData[0]=0x66;
	tData[1]=0x99;

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);

	W25Q32_SEND(tData, 2);

    LL_GPIO_SetOutputPin(GPIOA, SPI1_CS_Pin);

    HAL_Delay(100);

}

void id(){
	uint8_t tData = 0x9F;
	uint8_t rData[3];

	LL_GPIO_ResetOutputPin(GPIOA,SPI1_CS_Pin);
	W25Q32_SEND(&tData, 1);
	W25Q32_RECEIVE(rData, 3);

    LL_GPIO_SetOutputPin(GPIOA, SPI1_CS_Pin);
}
