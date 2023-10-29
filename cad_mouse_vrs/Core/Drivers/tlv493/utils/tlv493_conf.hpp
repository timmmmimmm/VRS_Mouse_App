#ifndef TLV493D_CONF_HPP_INCLUDED
#define TLV493D_CONF_HPP_INCLUDED

#include "RegMask.h"
#include "../tlv493.hpp"


#define TLV493D_DEFAULTMODE			POWERDOWNMODE

#define TLV493D_STARTUPDELAY		40
#define TLV493D_RESETDELAY			60

#define TLV493D_NUM_OF_REGMASKS		25
#define TLV493D_NUM_OF_ACCMODES		5

#define TLV493D_MEASUREMENT_READOUT	7
#define TLV493D_FAST_READOUT		3

#define TLV493D_B_MULT 				0.098
#define TLV493D_TEMP_MULT 			1.1
#define TLV493D_TEMP_OFFSET 		315


namespace tlv493d
{

typedef struct
{
	uint8_t fast;
	uint8_t lp;
	uint8_t lpPeriod;
	uint16_t measurementTime;
} AccessMode_t;

enum Registers_e
{
	R_BX1 = 0,
	R_BX2, 
	R_BY1, 
	R_BY2, 
	R_BZ1, 
	R_BZ2, 
	R_TEMP1, 
	R_TEMP2, 
	R_FRAMECOUNTER, 
	R_CHANNEL,
	R_POWERDOWNFLAG, 
	R_RES1,
	R_RES2,
	R_RES3,
	W_PARITY,
	W_ADDR,
	W_INT,
	W_FAST,
	W_LOWPOWER,
	W_TEMP_NEN,
	W_LP_PERIOD,
	W_PARITY_EN,
	W_RES1,
	W_RES2,
	W_RES3
};

//const RegMask_t regMasks[] = {
//	{ REGMASK_READ, 0, 0xFF, 0 },		// R_BX1
//	{ REGMASK_READ, 4, 0xF0, 4 },		// R_BX2
//	{ REGMASK_READ, 1, 0xFF, 0 },		// R_BY1
//	{ REGMASK_READ, 4, 0x0F, 0 },		// R_BY2
//	{ REGMASK_READ, 2, 0xFF, 0 },		// R_BZ1
//	{ REGMASK_READ, 5, 0x0F, 0 },		// R_BZ2
//	{ REGMASK_READ, 3, 0xF0, 4 },		// R_TEMP1
//	{ REGMASK_READ, 6, 0xFF, 0 },		// R_TEMP2
//	{ REGMASK_READ, 3, 0x0C, 2 },		// R_FRAMECOUNTER
//	{ REGMASK_READ, 3, 0x03, 0 },		// R_CHANNEL
//	{ REGMASK_READ, 5, 0x10, 4 },		// R_POWERDOWNFLAG
//	{ REGMASK_READ, 7, 0x18, 3 },		// R_RES1
//	{ REGMASK_READ, 8, 0xFF, 0 },		// R_RES2
//	{ REGMASK_READ, 9, 0x1F, 0 },		// R_RES3
//	{ REGMASK_WRITE, 1, 0x80, 7 },		// W_PARITY
//	{ REGMASK_WRITE, 1, 0x60, 5 },		// W_ADDR
//	{ REGMASK_WRITE, 1, 0x04, 2 },		// W_INT
//	{ REGMASK_WRITE, 1, 0x02, 1 },		// W_FAST
//	{ REGMASK_WRITE, 1, 0x01, 0 },		// W_LOWPOWER
//	{ REGMASK_WRITE, 3, 0x80, 7 },		// W_TEMP_EN
//	{ REGMASK_WRITE, 3, 0x40, 6 },		// W_LOWPOWER
//	{ REGMASK_WRITE, 3, 0x20, 5 },		// W_POWERDOWN
//	{ REGMASK_WRITE, 1, 0x18, 3 },		// W_RES1
//	{ REGMASK_WRITE, 2, 0xFF, 0 },		// W_RES2
//	{ REGMASK_WRITE, 3, 0x1F, 0 }		// W_RES3
//};

const AccessMode_t accModes[] = {
	{ 0, 0, 0, 1000 },		// POWERDOWNMODE
	{ 1, 0, 0, 0 },			// FASTMODE
	{ 0, 1, 1, 10 },		// LOWPOWERMODE
	{ 0, 1, 0, 100 },		// ULTRALOWPOWERMODE
	{ 1, 1, 1, 10 }			// MASTERCONTROLLEDMODE
};

}

#endif
