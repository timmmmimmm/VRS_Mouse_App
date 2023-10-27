/*
 * Tlv493d.cpp
 *
 *  Created on: Oct 24, 2023
 *      Author: laptop
 */
#include "Tlv493d.hpp"
#include "utils/RegMask.h"
#include "utils/tlv493_conf.hpp"

Tlv493d::tlv493(void)
{
	x_data = 0;
	y_data = 0;
	z_data = 0;
	temp_data = 0;
	expected_frame_count = 0x00;
}
Tlv493d::~tlv493(void){
	
}

TLV_ERRORS tlv493_init(TLV_ADDRESS slave_address,bool reset){

	initInterface(&mInterface, &bus, slaveAddress);
	delay(TLV493D_STARTUPDELAY);

	mInterface.bus->begin();

	if(reset == true)
	{
		resetSensor(mInterface.adress);
	}

	// get all register data from sensor
	tlv493d::readOut(&mInterface);
	// copy factory settings to write registers
	set_reg_bits(tlv493d::W_RES1, get_reg_bits(tlv493d::R_RES1));
	set_reg_bits(tlv493d::W_RES1, get_reg_bits(tlv493d::R_RES1));
	set_reg_bits(tlv493d::W_RES1, get_reg_bits(tlv493d::R_RES1));
	// enable parity detection
	set_reg_bits(tlv493d::W_PARITY_EN, 1);
	// config sensor to lowpower mode
	// also contains parity calculation and writeout to sensor
	setAccessMode(TLV493D_DEFAULTMODE);
	//test
	return TLV493_OK;
}

bool Tlv493d::setAccessMode(AccessMode_e mode)
{
	bool ret = BUS_ERROR;
	const tlv493d::AccessMode_t *modeConfig = &(tlv493d::accModes[mode]);
	set_reg_bits(tlv493d::W_FAST, modeConfig->fast);
	set_reg_bits(tlv493d::W_LOWPOWER, modeConfig->lp);
	set_reg_bits(tlv493d::W_LP_PERIOD, modeConfig->lpPeriod);
	calc_parity();
	ret = tlv493d::writeOut(&mInterface);
	if ( ret != BUS_ERROR )
	{
		mMode = mode;
	}
	return ret;
}
void Tlv493d::enableInterrupt(void)
{
	set_reg_bits(tlv493d::W_INT, 1);
	calc_parity();
	tlv493d::writeOut(&mInterface);
}


void Tlv493d::disableInterrupt(void)
{
	set_reg_bits(tlv493d::W_INT, 0);
	calc_parity();
	tlv493d::writeOut(&mInterface);
}

void Tlv493d::enableTemp(void)
{
	set_reg_bits(tlv493d::W_TEMP_NEN, 0);
	calc_parity();
	tlv493d::writeOut(&mInterface);
}


void Tlv493d::disableTemp(void)
{
	set_reg_bits(tlv493d::W_TEMP_NEN, 1);
	calc_parity();
	tlv493d::writeOut(&mInterface);
}


uint16_t Tlv493d::getMeasurementDelay(void)
{
	return tlv493d::accModes[mMode].measurementTime;
}

Tlv493d_Error_t Tlv493d::updateData(void)
{
	Tlv493d_Error_t ret = TLV493D_NO_ERROR;
	// in POWERDOWNMODE, sensor has to be switched on for one measurement
	uint8_t powerdown = 0;
	if(Mode == POWERDOWNMODE) 
	{
		if (setAccessMode(MASTERCONTROLLEDMODE) != BUS_OK)
		{
			ret = TLV493D_BUS_ERROR;
		}
		delay(getMeasurementDelay());
		powerdown = 1;
	}
	if(ret == TLV493D_NO_ERROR)
	{
		if (readOut(&mInterface, TLV493D_MEASUREMENT_READOUT) != BUS_OK)
		{
			ret = TLV493D_BUS_ERROR;
		}
		if (ret == TLV493D_NO_ERROR)
		{
			// construct results from registers
			x_data = concat_results(get_reg_bits(tlv493d::R_BX1), get_reg_bits(tlv493d::R_BX2), true);
			y_data = concat_results(get_reg_bits(tlv493d::R_BY1), get_reg_bits(tlv493d::R_BY2), true);
			z_data = concat_results(get_reg_bits(tlv493d::R_BZ1), get_reg_bits(tlv493d::R_BZ2), true);
			// switch sensor back to POWERDOWNMODE, if it was in POWERDOWNMODE before
			if(powerdown)
			{
				if (setAccessMode(POWERDOWNMODE) != BUS_OK)
				{
					ret = TLV493D_BUS_ERROR;
				}
			}
			if (ret == TLV493D_NO_ERROR)
			{
				// if the return value is 0, all results are from the same frame
				// otherwise some results may be outdated
				if(get_reg_bits(tlv493d::R_CHANNEL) != 0)
				{
					ret = TLV493D_FRAME_ERROR;
				}
			}
		}
	}
	mExpectedFrameCount = get_reg_bits(tlv493d::R_FRAMECOUNTER) + 1;
	return ret;
}


float Tlv493d::getX(void){
	return static_cast<float>(x_data) * TLV493D_B_MULT;
}
float Tlv493d::getY(void){
	return static_cast<float>(y_data) * TLV493D_B_MULT;
}
float Tlv493d::getZ(void){
	return static_cast<float>(z_data) * TLV493D_B_MULT;
}
/* internal function called by begin()
 * The sensor has a special reset sequence which allows to change its i2c address by setting SDA to high or low during a reset. 
 * As some i2c peripherals may not cope with this, the simplest way is to use for this very few bytes bitbanging on the SCL/SDA lines.
 * Furthermore, as the uC may be stopped during a i2c transmission, a special recovery sequence allows to bring the bus back to
 * an operating state.
 */

void Tlv493d::resetSensor(uint8_t adr)     // Recovery & Reset - this can be handled by any uC as it uses bitbanging
{
	mInterface.bus->beginTransmission(0x00);

	if (adr == TLV493D_ADDRESS1) {
		// if the sensor shall be initialized with i2c address 0x1F
		mInterface.bus->write(0xFF);
	} else {
		// if the sensor shall be initialized with address 0x5E
		mInterface.bus->write((uint8_t)0x00);
	}

	mInterface.bus->endTransmission(true);
}
void Tlv493d::set_reg_bits(uint8_t regMaskIndex, uint8_t data)
{
	if(regMaskIndex < TLV493D_NUM_OF_REGMASKS)
	{
		tlv493d::set_to_regs(&(tlv493d::regMasks[regMaskIndex]), mInterface.regWriteData, data);
	}
}
uint8_t Tlv493d::get_reg_bits(uint8_t regMaskIndex)
{
	if(regMaskIndex < TLV493D_NUM_OF_REGMASKS)
	{
		const tlv493d::RegMask_t *mask = &(tlv493d::regMasks[regMaskIndex]);
		if(mask->rw == REGMASK_READ)
		{
			return tlv493d::get_from_regs(mask, mInterface.regReadData);
		}
		else
		{
			return tlv493d::get_from_regs(mask, mInterface.regWriteData);
		}
	}
	return 0;
}
void Tlv493d::calc_parity(void) 
{
	uint8_t i;
	uint8_t y = 0x00;
	// set parity bit to 1
	// algorithm will calculate an even parity and replace this bit, 
	// so parity becomes odd
	set_reg_bits(tlv493d::W_PARITY, 1);
	// combine array to one byte first
	for(i = 0; i < TLV493D_BUSIF_WRITESIZE; i++)
	{
		y ^= mInterface.regWriteData[i];
	}
	// combine all bits of this byte
	y = y ^ (y >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);
	// parity is in the LSB of y
	set_reg_bits(tlv493d::W_PARITY, y&0x01);
}
int16_t Tlv493d::concat_results(uint8_t upperByte, uint8_t lowerByte, bool upperFull){
	int16_t value=0x0000;	//16-bit signed integer for 12-bit values of sensor
	if(upperFull)               
	{
		value=upperByte<<8;
		value|=(lowerByte&0x0F)<<4;
	}
	else
	{
		value=(upperByte&0x0F)<<12;
		value|=lowerByte<<4;
	}
	value>>=4;				//shift left so that value is a signed 12 bit integer
	return value;
}






