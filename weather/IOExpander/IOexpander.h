/*
 * IOexpander.h
 *
 * Created: 1/18/2024 6:06:57 PM
 *  Author: asase
 */ 

#ifndef IOEXPANDER_H_
#define IOEXPANDER_H_

#define MCP23008_ID 0x40	// MCP23008 Device Identifier
#define MCP23008_ADDR 0x00  // MCP23008 Device Address
#define IODIR 0x00          // MCP23008 I/O Direction Register
#define GPIO 0x09           // MCP23008 General Purpose I/O Register
#define OLAT 0x0A           // MCP23008 Output Latch Register

#include "../i2c/protocol_i2c.h"

void Write_MCP23008(unsigned char reg_addr,unsigned char data);

#endif /* IOEXPANDER_H_ */