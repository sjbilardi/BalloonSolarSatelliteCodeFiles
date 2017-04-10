/********************
This program uses interrupts to detect objects within 30cm.
********************/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL
#define UART_BAUD 4800
//Define Baud Rate as described in AVR manual
#define UART_UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

// Following are some function prototypes
ISR(ADC_vect);
ISR(TIMER1_COMPA_vect);
void initADC(); // initialize ADC
void setAnalogPort(int channel);
double calcDistance(uint16_t IRVal);
void uart_init(); // This function initializes the AVR UART
void write_uart(unsigned char c[]); //This function writes a string to the uart


int main()
{	
	/* Initialize UART */
	uart_init();
	
	/* Setup Internal Clock Interrupt */
	TCCR1B |= (1<<WGM12);		// CTC with OCR1A TOP
	sei();						// Enable global interrupts
	OCR1A = (1000000/1024);		// Timer1 should reach TOP every second
	TIMSK1 |= (1<<OCIE1A);						// Output compare interrupt
	TCCR1B |= (1<<CS10) | (1<<CS12);			// CLK/1024
	
	/* Initialize ADC */
	int IRChannel = 0;
	initADC();					// initialize ADC
	setAnalogPort(IRChannel);	// set port number to read
	ADCSRA |= (1<<ADIE);		// enable ADC interrupt flag
	ADCSRA |= (1<<ADATE);		// enable auto enable
	ADCSRA |= (1<<ADSC);		// Start ADC
	
	while(1){}					// do nothing in while loop. interrupts will take care of everything
}

ISR(ADC_vect)				// When each ADC completes, this routine is executed
{
	double ir = calcDistance((ADCL | (ADCH<<8)));
	if (ir < 30.0)          // When something is close to the IR sensor
	{
		char buffer[40];
		char distanceString[10];
		ADCSRA &= ~(1<<ADEN); // Disable ADC
		dtostrf(ir, 6, 3, distanceString);
		sprintf(buffer,"Time %i ms, ", TCNT1);   // TCNT1 is the count of timer1
		sprintf(buffer + strlen(buffer), "Dist: ");
		sprintf(buffer + strlen(buffer), distanceString);
		sprintf(buffer + strlen(buffer), "\n\r"); 
		write_uart(buffer);
		TCNT1=0;				// Reset the timer back to zero
		_delay_ms(200);			//Wait some time to start proximity sensing again
		ADCSRA |= (1<<ADEN);	// Enable ADC
		ADCSRA |= (1<<ADSC);	// Start ADC
	}
}

ISR(TIMER1_COMPA_vect)   // this interrupt is fired when timer 1 equals OCR1A
{
	write_uart("Timer reset.\n\r"); // Write the string to the UART
}

void initADC()
{
	// initializes ADC for 10-bit reading
	
	// Set ADC prescalar to 8: Which is 125KHz sample rate @ 1MHz
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);
	
	ADMUX |= (1<<REFS0); // This sets ADC reference to AVCC
	
	// enable the ADC
	ADCSRA |= (1<<ADEN); // Enable ADC
}

void setAnalogPort(int channel)
{
	ADMUX = 0x40; // Reset ADMUX to reset channel to zero
	ADMUX |= channel; // set channel to MUX
}

double calcDistance(uint16_t IRVal)
{
	double Val = (double)IRVal; // make double version of IRVal
	double distance;
	
	distance = 9801.8*pow(Val, -1.127); // use power function to calibrate ADC to distance (cm)
	
	return distance;
}

void uart_init() // initiatlize UART
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