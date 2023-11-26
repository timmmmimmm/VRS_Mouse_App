#ifndef TLV493C_H_INCLUDE
#define TLV493C_H_INCLUDE

#include "i2c.h"
#include "usart.h"
#include <math.h>

#define TLV493D_B_MULT 		0.098
#define TLV493_ADDRESS     (0x5E << 1)
/**
 * @note READ REGISTERS
 * 
*/
#define TLV493_BX1          0x0
#define TLV493_BY1          0x1
#define TLV493_BZ1          0x2
#define TLV493_BX2          0x4
#define TLV493_BZ2          0x5

/**
 * @note WRITE REGISTERS
 * 
*/
#define TLV493_MOD1         0x1
#define TLV493_MOD2         0x3

uint16_t dataX, dataY, dataZ;
/**
 * @brief Function for initialization tlv493D sensor which meassure rotation of magnet in 3-axis
 * 
 * @return 1 when sensor init ok 0 when is error while init
*/
uint8_t tlv493_init();

/**
 * @brief function for reading multiple bytes with i2c from sensor
 * @param reg_address address of register read from 
 * @param data pointer to variable to store data in
 * @param len number of bytes 
 * 
*/
void tlv493_read_bytes(uint8_t reg_address, uint8_t *data, uint8_t len);
/**
 * @brief function for write multiple bytes with i2c from sensor
 * @param reg_address address of register read from 
 * @param data pointer to variable store data in
 * @param len number of bytes 
 * 
*/
void tlv493_write_bytes(uint8_t reg_address, uint8_t *data, uint8_t len);

void tlv493_update_data();
void tlv493_test();
/**
 * @brief read position of magnet on axis
 * @return position of magnet on axis
*/
float getX();
float getY();
float getZ();
void get_data(float *coord);

#endif
