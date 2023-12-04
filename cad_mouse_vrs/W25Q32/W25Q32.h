#ifndef W25Q32_H_
#define W25Q32_H_
#include "spi.h"

#define READ_INSTRUCTION 0x03U
#define WRITE_INSTRUCTION 0x02U
#define SECTOR_ERASE_INSTRUCTION 0x20U
#define WRITE_ENABLE_INSTRUCTION 0x06U
#define WRITE_DISABLE_INSTRUCTION 0x04U
#define ID_INSTRUCTION 0x9FU

#define WRITE_ADDRESS_MSB 0x3FU
#define WRITE_ADDRESS_MID 0xFFU  //for testing
#define WRITE_ADDRESS_LSB 0x00U

#define W25Q32_ID 0x4016U


void W25Q32_WRITE_DPI(uint8_t dpi);
uint8_t W25Q32_READ_DPI();

static void (*W25Q32_RECEIVE)(uint8_t* receive_data, uint32_t size)=0;
static void (*W25Q32_SEND)(uint8_t* send_data, uint32_t size)=0;
static void (*W25Q32_CS_LOW)()=0;
static void (*W25Q32_CS_HIGH)()=0;
static void (*W25Q32_DELAY)(uint16_t delay)=0;

void W25Q32_REGISTERCALLBACKS(void *receive_callback, void *send_callback, void *cs_low_callback, void *cs_high_callback, void *delay_callback);
#endif

