/*
 * tlv493.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: laptop
 */
#include "tlv493.hpp"

TLV_ERRORS tlv493_init(TLV_ADDRESS slave_address,bool reset){

	MX_I2C1_Init();


	return TLV493_OK;
}





