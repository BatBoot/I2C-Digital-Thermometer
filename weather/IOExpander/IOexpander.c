#include "IOexpander.h"

void Write_MCP23008(unsigned char reg_addr,unsigned char data)
{
	i2c_start(MCP23008_ID, MCP23008_ADDR, TW_WRITE);		// Start the I2C Write Transmission
	i2c_write(reg_addr);									// Sending the Register Address
	i2c_write(data);										// Write data to MCP23008 Register
	i2c_stop();												// Stop I2C Transmission
}
