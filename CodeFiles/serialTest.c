/*
 * GccApplication1.c
 *
 * Created: 4/21/2017 10:14:30 AM
 * Author : Sergei
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#define UART_BAUD 4800
//Define Baud Rate as described in AVR manual
#define UART_UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

void uart_init() // initialize UART
{
	/*Set Baud rate*/
	UBRR0H = UART_UBRR_VALUE >> 8;  // set high byte
	UBRR0L = UART_UBRR_VALUE;  // set low bytes
	/*Frame format: 8 data bits, no parity, 1 stop bit*/
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	/*Enable Transmit and Receive*/
	UCSR0B = (1<< RXEN0)|(1<<TXEN0);
}

void write_uart(unsigned char c[]) // transmit data over USART
{
	do
	{
		while((UCSR0A&(1<<UDRE0)) == 0); // Wait for Transmit Buffer to Empty
		UDR0 = *c; // Transmit the character
		c++; // Increment the pointer to point to the next character
	}while(*c != '\0');
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		write_uart("22.12 23.21 9.5 900 20.0 100");
		
		/*
		String order:
		
			- Temp1 (degrees C)
			- temp2 (degrees C)
			- Battery Voltage (V)
			- Battery Current Draw (mA)
			- Sun Angle (degrees)
			- Comments (read as integer) 
		*/
    }
}

