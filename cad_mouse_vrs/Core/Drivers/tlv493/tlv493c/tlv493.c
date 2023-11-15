#include "tlv493.h"


static uint8_t slave_address = TLV493_ADDRESS0;
void tlv493_write_bytes(uint8_t reg_address, uint8_t *data, uint8_t len){
	reg_address |= (1<<7);
    i2c_master_write(data, reg_address, slave_address, len);
}
void tlv493_read_bytes(uint8_t reg_address, uint8_t *data, uint8_t len){
	reg_address |= (1<<7);
    i2c_master_read(data, reg_address, slave_address, len);
}

uint8_t tlv493_init(){
    

    uint8_t MOD2 = 0b0110000;
    tlv493_write_bytes(TLV493_MOD2, &MOD2, 1);
    return 1;
}

float getX(){
    uint8_t data[6];
    tlv493_read_bytes(TLV493_BX1, &data,6);
    uint16_t cast = (uint16_t)((data[0] << 8)) | (data[4] >> 4);
    return (float)(cast)*TLV493D_B_MULT;
}
float getY(){
    uint8_t data[5];
    tlv493_read_bytes(TLV493_BX1, &data,6);
    uint16_t cast = (uint16_t)(data[1] << 8 | (data[4] & 0b1111) << 8);
    return (float)cast*TLV493D_B_MULT;
}
float getZ(){
    uint8_t data[5];
    tlv493_read_bytes(TLV493_BX1, &data,6);
    uint16_t cast = (uint16_t)(data[3] << 8) | data[5] & 0b1111;
    return (float)cast*TLV493D_B_MULT;
}
