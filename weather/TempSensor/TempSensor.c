#include "TempSensor.h"

void DS1621_init(void)
{
	 i2c_start(DS1621_ID, DS1621_ADDR, TW_WRITE);	//Start the I2C communication
	 i2c_write(DS1621_CONFIG);						//Give the config register
	 i2c_write(CONT_CONV);							//Command for continuous conversion mode
	 i2c_start(DS1621_ID, DS1621_ADDR, TW_WRITE);   //Hold the data bus
	 i2c_write(START_CONV);							//Start converting
	 i2c_stop();
}

int16_t DS1621_getCTemp()
{
	uint8_t msb, lsb;								//Uint8_t used for storing a numerical value
	i2c_start(DS1621_ID, DS1621_ADDR, TW_WRITE);	//Take over the bus
	i2c_write(READ_TEMP);							//Send the temperature reading register
	i2c_start(DS1621_ID, DS1621_ADDR, TW_READ);		//Repeated start to keep the data bus
	msb = I2C_read_ack();							//Read the MSB with acknowledge
	lsb = I2C_read_nack();							//Get the LSB without acknowledge
	i2c_stop();										//Release the data bus
	
	int16_t temp = (int8_t) msb << 1 | (lsb >> 7);	//Get the full temperature by creating the data byte
	temp = temp * 5;								//Convert to tenth of Celsius degrees
	return temp;									//return the value
}