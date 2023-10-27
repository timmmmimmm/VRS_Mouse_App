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

enum TLV_ADDRESS{
    TLV493_ADDRESS1 = 0x5E,
    TLV493_ADDRESS2 = 0x1F
};

enum TLV_ERRORS{
    TLV493_OK = 0,
    TLV493_BUS_ERROR,
    TLV493_FRAME_ERROR
};


class Tlv493d
{

public:
    
    tlv493(void);
    ~tlv493();
    TLV_ERRORS tlv493_init(TLV_ADDRESS slave_address,bool reset);
    
    enum AccessMode{
        POWERDOWNMODE = 0,
		FASTMODE,
		LOWPOWERMODE,
		ULTRALOWPOWERMODE,
		MASTERCONTROLLEDMODE,
    };
    bool setAccessMode(AccessMode mode);
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
    AccessMode Mode;
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
};

Tlv493d::tlv493()
{
}

Tlv493d::~tlv493()
{
}

#endif /* DRIVERS_TLV493_TLV493_HPP_ */
