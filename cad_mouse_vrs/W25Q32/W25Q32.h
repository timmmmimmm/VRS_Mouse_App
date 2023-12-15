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

void W25Q32_GET_ID();
void W25Q32_RESET();
void W25Q32_READ_DATA(uint32_t startPage,uint8_t offset, uint8_t size, uint8_t * rData);
void W25Q32_WRITE_DATA(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
void W25Q32_WRITE_ENABLE();
void W25Q32_WRITE_DISABLE();
void W25Q32_ERASE_SECTOR(uint16_t numsector);
void W25Q32_REGISTER_CALLBACKS(void *receive_callback, void *send_callback, void *cs_low_callback, void *cs_high_callback, void *delay_callback);

uint16_t W25Q32_READ_DPI();
void W25Q32_WRITE_DPI(uint16_t dpi);
uint8_t W25Q32_READ_ACTION_BUTTON_0();
void W25Q32_WRITE_ACTION_BUTTON_0(uint8_t action_button);
uint8_t W25Q32_READ_ACTION_BUTTON_1();
void W25Q32_WRITE_ACTION_BUTTON_1(uint8_t action_button);



uint32_t bytes_to_write(uint32_t size,uint16_t offset);

static void (*W25Q32_RECEIVE)(uint8_t* receive_data, uint32_t size)=0;
static void (*W25Q32_SEND)(uint8_t* send_data, uint32_t size)=0;
static void (*W25Q32_CS_LOW)()=0;
static void (*W25Q32_CS_HIGH)()=0;
static void (*W25Q32_DELAY)(uint16_t delay)=0;

#endif

