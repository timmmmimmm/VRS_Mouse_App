/*
 * tlv493.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: laptop
 */
#include "tlv493.hpp"

TLV_ERRORS tlv493_init(TLV_ADDRESS slave_address,bool reset){

	MX_GPIO_Init();
	MX_I2C1_Init();
//	if(i2c_master_read_byte(TLV493_ADDRESS1, LSM6DS0_WHO_AM_I_ADDRES) == LSM6DS0_WHO_AM_I_VALUE)
	return TLV493_OK;
}





