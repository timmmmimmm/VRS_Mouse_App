#ifndef TLV493C_H_INCLUDE
#define TLV493C_H_INCLUDE

#include "i2c.h"
#include <math.h>

#define TLV493D_B_MULT 		0.098
#define TLV493_ADDRESS0     0x5E
#define TLV493_ADDRESS1     0x1F

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


#endif