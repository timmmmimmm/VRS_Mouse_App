#include "tlv493.h"

uint16_t dataX, dataY, dataZ;
float cal_x=0, cal_y=0, cal_z=0;

int16_t powTable[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, -2048};

static HAL_StatusTypeDef(* i2cRead)(uint8_t registerAddress, uint8_t *data, uint8_t len);
static HAL_StatusTypeDef(* i2cWrite)(uint8_t registerAddress, uint8_t *data, uint8_t len);
static void(* delay)(uint32_t ms);

typedef enum
{
	X = 0,
	Y,
	Z
}AXIS;

/**
 * @brief function for write multiple bytes with i2c from sensor
 * @param reg_address address of register read from
 * @param data pointer to variable store data in
 * @param len number of bytes
 *
*/
static void tlv493_write_bytes(uint8_t *data, uint8_t len){
    (void) i2cWrite(TLV493_ADDRESS, data, len);
}

/**
 * @brief function for reading multiple bytes with i2c from sensor
 * @param reg_address address of register read from
 * @param data pointer to variable to store data in
 * @param len number of bytes
 *
*/
static void tlv493_read_bytes( uint8_t *data, uint8_t len){
    (void) i2cRead(TLV493_ADDRESS, data, len);
}

static void tlv493_reset(){
	(void) i2cWrite(TLV493_ADDR_RESET,0,0);
}

static float getAxis(AXIS axis);

uint8_t tlv493_init(HAL_StatusTypeDef(* i2cReadFn)(uint8_t registerAddress, uint8_t *data, uint8_t len),
					HAL_StatusTypeDef(* i2cWriteFn)(uint8_t registerAddress, uint8_t *data, uint8_t len),
					void(* delayFn)(uint32_t ms))
{

	i2cRead = i2cReadFn;
	i2cWrite = i2cWriteFn;
	delay = delayFn;

	tlv493_reset();



	uint8_t metaDataBuff[10] = {0};
	tlv493_read_bytes(metaDataBuff, 10);

	uint8_t setupDataBuff[4] = {0};
	setupDataBuff[1] = (metaDataBuff[7] & 0x18) | (uint8_t)TLV493_ITEn_LP;
	setupDataBuff[2] = metaDataBuff[8];
	setupDataBuff[3] = (uint8_t)TLV493_TempOff_LP_PT | (metaDataBuff[9] & 0x0F);

	tlv493_write_bytes(setupDataBuff, 2);
	delay(1);
	return 1;
}

void tlv493_update_data(){
	uint8_t data[6] = {0};
    uint8_t prevData = 0;

    while(prevData == (data[3] & 0x0C)) //Check if the frame advanced
    {
    	prevData = (data[3] & 0x0C);
    	tlv493_read_bytes(data,6);
    	delay(5);
    }


    dataX = (uint16_t)(data[0] << 4) | (uint16_t)(data[4] >> 4);
    dataY = (uint16_t)(data[1] << 4) | (uint16_t)(data[4] & 0x0F);
    dataZ = (uint16_t)(data[2] << 4) | (uint16_t)(data[5] & 0x0F);
}

float tlv493_getX()
{
	return getAxis(X);
}

float tlv493_getY(){
	return getAxis(Y);
}

float tlv493_getZ(){
	return getAxis(Z);
}


float getAxis(AXIS axis){
	uint16_t* currentAxis = 0;

	switch (axis) {
		case X:
			currentAxis = &dataX;
			break;
		case Y:
			currentAxis = &dataY;
			break;
		case Z:
			currentAxis = &dataZ;
			break;
		default:
			currentAxis = 0;
			break;
	}

	int16_t res = 0;
	for (int i = 0; i < 12; ++i) {

			if(*currentAxis & (1 << i)) // TLV Manual Page 9 - Table 1
			{
				res += powTable[i];
			}
		}

	return (float)res * TLV493D_B_MULT;
}
