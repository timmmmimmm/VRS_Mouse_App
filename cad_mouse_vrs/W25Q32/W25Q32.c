/*
 * W25Q32.c
 *
 *  Created on: Nov 20, 2023
 *      Author: edisimo
 */

#include "W25Q32.h"

//will have to rename those functions i guess..

void W25Q32_REGISTER_CALLBACKS(void *receive_callback, void *send_callback, void *cs_low_callback, void *cs_high_callback, void *delay_callback){
	if (receive_callback!=0){
			W25Q32_RECEIVE=receive_callback;
		}
	if (send_callback!=0){
			W25Q32_SEND=send_callback;
		}
	if (cs_low_callback!=0){
			W25Q32_CS_LOW=cs_low_callback;
	}
	if (cs_high_callback!=0){
			W25Q32_CS_HIGH=cs_high_callback;
		}
	if (delay_callback!=0){
			W25Q32_DELAY=delay_callback;
	}
}

void W25Q32_ERASE_SECTOR(uint16_t numsector){
	uint8_t tData[4];
	uint32_t memAddr = numsector*256*16; //16 pages, 256 bytes
	uint8_t se=SECTOR_ERASE_INSTRUCTION;

	W25Q32_WRITE_ENABLE();

	tData[0]=se;
	tData[1]=(memAddr >> 16)&0xFF; //MSB
	tData[2]=(memAddr >> 8)&0xFF; //MID
	tData[3]=(memAddr)&0xFF; //LSB

	W25Q32_CS_LOW();

	W25Q32_SEND(tData, 4);

	W25Q32_CS_HIGH();
	W25Q32_DELAY(450);

	W25Q32_WRITE_DISABLE();
}

uint32_t bytes_to_write(uint32_t size,uint16_t offset){
	if ((size+offset)<256) return size;
	else return 256-offset;
}


void W25Q32_WRITE_DATA(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data){
	uint8_t en = WRITE_INSTRUCTION;
	uint8_t tData[266];
	uint32_t startPage = page;
	uint32_t endPage = startPage + ((size+offset-1)/256);
	uint32_t numPages = endPage-startPage+1;

	uint16_t startSector = startPage/16;
	uint16_t endSector = endPage/16;
	uint16_t numSectors = endSector - startSector + 1;

	for(uint16_t i=0;i<numSectors; i++){
		W25Q32_ERASE_SECTOR(startSector++);
	}

	uint32_t dataPosition = 0;

	for(uint32_t i=0;i<numPages; i++){
		uint32_t memAddr = (startPage*256)+offset;
		uint16_t bytesremaining = bytes_to_write(size, offset);

		W25Q32_WRITE_ENABLE();

		tData[0]=en;
		tData[1]=(memAddr >> 16)&0xFFU; //MSB
		tData[2]=(memAddr >> 8)&0xFFU; //MID
		tData[3]=(memAddr)&0xFFU; //LSB
		uint32_t index = 4;

		uint16_t bytestosend = bytesremaining + index;

		for (uint16_t i=0; i < bytesremaining; i++){
			tData[index++]=data[i+dataPosition];
		}
		W25Q32_CS_LOW();
		W25Q32_SEND(tData, bytestosend);
		W25Q32_CS_HIGH();

		startPage++;
		offset=0;
		size = size - bytesremaining;
		dataPosition = dataPosition + bytesremaining;

		W25Q32_DELAY(5);
		W25Q32_WRITE_DISABLE();
	}
}

void W25Q32_WRITE_ENABLE(){
	uint8_t en=WRITE_ENABLE_INSTRUCTION;

	W25Q32_CS_LOW();

	W25Q32_SEND(&en, 1);
	W25Q32_CS_HIGH();
	W25Q32_DELAY(10);
}

void W25Q32_WRITE_DISABLE(){
	uint8_t en=WRITE_DISABLE_INSTRUCTION;

	W25Q32_CS_LOW();

	W25Q32_SEND(&en, 1);
	W25Q32_CS_HIGH();
	W25Q32_DELAY(10);
}


void W25Q32_READ_DATA(uint32_t startPage,uint8_t offset, uint8_t size, uint8_t * rData){
	uint8_t en = READ_INSTRUCTION;
	uint32_t address = (startPage*256)+offset;

	uint8_t tData[4];
	tData[0]=en;
	tData[1]=(address >> 16)&0xFFU; //MSB
	tData[2]=(address >> 8)&0xFFU; //MID
	tData[3]=(address)&0xFFU; //LSB

	W25Q32_CS_LOW();

	W25Q32_SEND(tData, 4);
	W25Q32_DELAY(10);
	W25Q32_RECEIVE(rData, size);


	W25Q32_CS_HIGH();

	W25Q32_DELAY(100);
}


void W25Q32_RESET(){
	uint8_t tData[2];
	tData[0]=0x66;
	tData[1]=0x99;

	W25Q32_CS_LOW();

	W25Q32_SEND(tData, 2);

    W25Q32_CS_HIGH();

    W25Q32_DELAY(100);

}

void W25Q32_GET_ID(){
	uint8_t tData = 0x9F;
	uint8_t rData[3];

	W25Q32_CS_LOW();

	W25Q32_SEND(&tData, 1);
	W25Q32_RECEIVE(rData, 3);

	W25Q32_CS_HIGH();
}
