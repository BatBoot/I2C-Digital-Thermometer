#ifndef PROTOCOL_I2C_H_
#define PROTOCOL_I2C_H_

#define MAX_TRIES 50		//Limit for the communication tries
#define I2C_START 0			//I2C start signal
#define I2C_DATA 1			//I2C data without acknowledge signal
#define I2C_DATA_ACK 2		//I2C data with acknowledge signal
#define I2C_STOP 3			//I2C stop communication signal
#define ACK 1				//Acknowledge flag
#define NACK 0				//Not acknowledge flag
#define DATASIZE 32			//Max size of the data transfered through the data bus

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

void i2c_init(void);
char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type);
char i2c_write(char data);
void i2c_stop(void);
unsigned char I2C_read_nack(void);
unsigned char I2C_read_ack(void);
unsigned char i2c_transmit(unsigned char type);

#endif /* PROTOCOL_I2C_H_ */