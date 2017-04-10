#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 1000000UL

#define UART_BAUD 4800
//Define Baud Rate as described in AVR manual
#define UART_UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

#define WAVESTEPMODE 0 // waveStepMode
#define FULLSTEPMODE 1 // fullStepMode
#define HALFSTEPMODE 2 // halfStepMode
	
/* Structures */
/* Motor */
typedef struct {
	int waveSteps[4];
	int fullSteps[4];
	int halfSteps[8];
	
	int waveStepsSize;
	int fullStepsSize;
	int halfStepsSize;
	
	int mode;
	
	int pins;
	} Motor;

/* Motor Functions */
Motor motor_init(int pins);
void moveMotor(Motor *motor);

/* Serial Comm Functions */
void uart_init(); // This function initializes the AVR UART
void write_uart(unsigned char c[]);		//This function writes a string to the uart

int main()
{
	char buffer[20];

	uart_init();				// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init(0x0F);		// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	
	while(1)
	{
		moveMotor(&motor); // run through one cycle
	}

}

Motor motor_init(int pins)
{
	Motor motor = {	{0x08, 0x02, 0x04, 0x01},						// waveSteps
					{0x0A, 0x06, 0x05, 0x09},				// fullSteps
					{0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x01, 0x09},	// halfSteps
					4, // waveStepsSize
					4, // fullStepsSize
					8, // halfStepsSize
					FULLSTEPMODE, // mode (default = fullStepMode)
					pins}; // PORTB pins 0-3
	DDRB |= pins; // set register B pins to high; configured as high
	return motor;
}

void moveMotor(Motor *motor)
{
	int i = 0;
	int *steps; 
	int numbStep;
	
	switch(motor->mode)
	{
		case WAVESTEPMODE:
			steps = &motor->waveSteps;
			numbStep = motor->waveStepsSize; 
			break;
		case FULLSTEPMODE:
			steps = &motor->fullSteps;
			numbStep = motor->fullStepsSize; 
			break;
		case HALFSTEPMODE:
			steps = &motor->halfSteps;
			numbStep = motor->halfStepsSize;
			break;
	}
	
	for(i=0; i<numbStep; i++)
	{
		PORTB &= 0xF0; // Turn off the stepper motor only
		_delay_ms(50); // Delay the time between turning off and on the motor
		PORTB |= *(steps + i); // Cause the stepper motor to make a step
		_delay_ms(50); // Delay the time between the step and shutting off the motor
	}
}

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