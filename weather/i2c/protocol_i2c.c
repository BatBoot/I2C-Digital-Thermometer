#include "protocol_i2c.h"

void i2c_init(void)		 // Initial ATMega328P TWI/I2C Peripheral
{
	TWSR = 0x00;         // Select Prescaler of 1
						 // SCL frequency = 11059200 / (16 + 2 * 48 * 1) = 98.743 kHz
	TWBR = 0x30;         // 48 Decimal
}

unsigned char i2c_transmit(unsigned char type)
{
		switch(type) 
		{
			case I2C_START:    // Send Start Condition
				TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
				break;
			case I2C_DATA:     // Send Data with No-Acknowledge
				TWCR = (1 << TWINT) | (1 << TWEN);
				break;
			case I2C_DATA_ACK: // Send Data with Acknowledge
				TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
				break;
			case I2C_STOP:     // Send Stop Condition
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				return 0;
		}
		
		while (!(TWCR & (1 << TWINT)));	// Wait for TWINT flag set on Register TWCR
		
		return (TWSR & 0xF8);			// Return TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
}

char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type)
{
		unsigned char n = 0;
		unsigned char twi_status;
		char r_val = -1;
		
i2c_retry:
		if (n++ >= MAX_TRIES) return r_val;
		
		twi_status = i2c_transmit(I2C_START);				//Lock in the data bus

	
		if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;	// Check the TWI Status
		if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
		
		TWDR = (dev_id & 0xF0) | (dev_addr & 0x0E) | rw_type; // Send slave address (SLA_W)
		
		twi_status = i2c_transmit(I2C_DATA);				// Transmit I2C Data
		
		if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
		if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;		// Check the TWSR status
		r_val = 0;
		
i2c_quit:
		return r_val;
}

char i2c_write(char data)
{
	unsigned char twi_status;
	char r_val = -1;
	
	TWDR = data;										// Send the Data to I2C Bus
	twi_status = i2c_transmit(I2C_DATA);				// Transmit I2C Data
	
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;	// Check the TWSR status
	r_val=0;
	
i2c_quit:
	return r_val;
}

unsigned char I2C_read_ack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Enable TWI, generation of ACK, clear interrupt flag
	while(!(TWCR & (1 << TWINT)));					 // Wait until TWI finishes reading
	
	return TWDR;									 // Return received data
}

unsigned char I2C_read_nack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);				 // Enable TWI and clear interrupt flag
	while(!(TWCR & (1 << TWINT)));					 // Wait until TWI finishes reading
	
	return TWDR;									 // Return received data
}

void i2c_stop(void) { i2c_transmit(I2C_STOP); }		 //Release the data bus	