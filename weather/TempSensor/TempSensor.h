#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_
#include "../i2c/protocol_i2c.h"

#define DS1621_ID 0x90				//Set address for the DS1621
#define DS1621_ADDR 0x00			//Configured address fro the DS1621
#define DS1621_CONFIG 0xAC			//Config register
#define CONT_CONV 0x00				//Continuous Conversion flag
#define START_CONV 0xEE				//Start the conversion flag
#define READ_TEMP 0xAA				//Read the temperature register

void DS1621_init(void);
int16_t DS1621_getCTemp();

#endif /* TEMPSENSOR_H_ */