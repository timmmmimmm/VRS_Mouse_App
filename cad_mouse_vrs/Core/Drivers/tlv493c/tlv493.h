#ifndef TLV493C_H_INCLUDE
#define TLV493C_H_INCLUDE

#include "i2c.h"
#include "usart.h"
#include <math.h>

#define TLV493D_B_MULT 			(float)0.098
#define TLV493_ADDRESS			(0x5E << 1)

#define TLV493_ADDR_RESET		(0x00)


#define TLV493_ITEn_LP			0x04
#define TLV493_TempOff_LP_PT 	0xC0


#define SAMPLES				256
/**
 * @brief Function for initialization tlv493D sensor which meassure rotation of magnet in 3-axis
 * 
 * @return 1 when sensor init ok 0 when is error while init
*/
uint8_t tlv493_init(HAL_StatusTypeDef(* i2cReadFn)(uint8_t registerAddress, uint8_t *data, uint8_t len),
		HAL_StatusTypeDef(* i2cWriteFn)(uint8_t registerAddress, uint8_t *data, uint8_t len),
		void(* delayFn)(uint32_t ms));

void tlv493_update_data();
//void tlv493_test();
/**
 * @brief read position of magnet on axis
 * @return position of magnet on axis
*/
float tlv493_getX();
float tlv493_getY();
float tlv493_getZ();
//void get_data(float *coord);

#endif
