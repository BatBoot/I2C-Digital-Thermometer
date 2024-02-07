#define F_CPU 16000000UL					//ATmega328p clock frequency
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2c/protocol_i2c.h"				//Custom library for the i2c protocol
#include "IOExpander/IOexpander.h"			//Custom library for the IO Expander
#include "TempSensor/TempSensor.h"			//Custom library for the DS1621


void USART_Init(void)						// Set baud rate:
{	
	UBRR0=103;								//UBRR= Fosc /(16*9600) -1 =103.166= 103
	UCSR0B |= (1 << RXEN0 | (1 << TXEN0));	// enable receiver and transmitter
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);	// Set frame format : 8 data 2 stop bit
}

void USART_Transmit( uint8_t data )
{
	while(!( UCSR0A & (1<<UDRE0)));			// Wait for empty transmit buffer 
	UDR0 = data;							// Put data into buffer, sends the data 
}

int main(void)
{
  USART_Init();								// Initial Master I2C
  i2c_init();
  Write_MCP23008(IODIR,0b00000000);			//Initial the MCP23008 GP0 to GP7 as Output
  Write_MCP23008(GPIO,0b00000000);			//Reset all the Output Port
  DS1621_init();							//Initialize the temperature sensor
  sei();									//Enable global interrupts

  while(1) 
  {
	  int16_t temp = DS1621_getCTemp();		//Get the temperature
	  Write_MCP23008(GPIO, ((temp / 10) << 1) | ((temp % 10) / 5));	//Display the temperature on the 8 LEDs
	  
	  char buffer[10];						//Buffer for the Serial monitor
	  snprintf(buffer, sizeof(buffer), "%u.%1uC\n", temp / 10, temp % 10); //Put the temperature in the buffer and format the text
	  for (int i = 0; i < strlen(buffer); i++) 
		  USART_Transmit(buffer[i]);		//Send the buffer to the Serial Monitor
	  
	  _delay_ms(1000);						//Delay for 1 sec.
  }
  
  return 0;
}
