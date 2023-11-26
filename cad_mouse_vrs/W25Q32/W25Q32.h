#ifndef W25Q32_H_
#define W25Q32_H_

#define READ_INSTRUCTION 0x03U
#define WRITE_INSTRUCTION 0x02U
#define SECTOR_ERASE_INSTRUCTION 0x20U
#define WRITE_ENABLE_INSTRUCTION 0x06U

#define WRITE_ADDRESS_MSB 0x3FU
#define WRITE_ADDRESS_MID 0xFFU
#define WRITE_ADDRESS_LSB 0x00U

static void (*W25Q32_READ)(uint8_t CS_PIN, uint8_t register_address, uint8_t length, uint8_t* write_data)=0;
static void (*W25Q32_WRITE)(uint8_t CS_PIN, uint8_t register_address, uint8_t length, uint8_t* read_data)=0;

#endif
