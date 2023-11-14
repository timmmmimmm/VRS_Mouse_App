/*
 * Tlv493d.hpp
 *
 *  Created on: Oct 24, 2023
 *      Author: laptop
 */

#ifndef DRIVERS_TLV493_TLV493_HPP_
#define DRIVERS_TLV493_TLV493_HPP_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"
#include "gpio.h"

#define TLV493_ADDRESS0     0x5E
#define TLV493_ADDRESS1     0x1F
#define WHO_AM_I            0b00111110
enum TLV_ERRORS{
    TLV493_OK = 0,
    TLV493_BUS_ERROR,
    TLV493_FRAME_ERROR
};


class Tlv493d
{

public:
    
//    void tlv493(void);
//    coid ~tlv493();
    TLV_ERRORS tlv493_init(bool reset);
   
    void enableInterrupt(void);
    void disableInterrupt(void);
    void enableTemp(void);
    void disableTemp(void);

	// returns the recommended time between two readouts for the sensor's current configuration
    uint16_t getMeasurementDelay(void);
    // read measurement results from sensor
	TLV_ERRORS updateData(void);

    // fieldvector in Cartesian coordinates
	float getX(void);
	float getY(void);
	float getZ(void);
	
	// fieldvector in spherical coordinates
	float getAmount(void);
	float getAzimuth(void);
	float getPolar(void);
	
	// temperature
	float getTemp(void);
private:
    int16_t x_data;
    int16_t y_data;
    int16_t z_data;
    int16_t temp_data;
    uint8_t expected_frame_count;

    void reset_sensor(uint8_t adr);
	void set_reg_bits(uint8_t regMaskIndex, uint8_t data);
	uint8_t get_reg_bits(uint8_t regMaskIndex);
	void calc_parity(void);
	int16_t concat_results(uint8_t upperByte, uint8_t lowerByte, bool upperFull);
    void tlv493_read_byte(uint8_t *data, uint8_t reg_address,uint8_t len);
    void tlv493_write_byte(uint8_t reg_address, uint8_t *data, uint8_t len);
};

//Tlv493d::tlv493()
//{
//}
//
//Tlv493d::~tlv493()
//{
//}

#endif /* DRIVERS_TLV493_TLV493_HPP_ */
