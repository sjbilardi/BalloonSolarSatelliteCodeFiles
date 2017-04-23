#include "commonHeaders.h"

void initADC()
{
	// initializes ADC for 10-bit reading

	// Set ADC prescalar to 8: Which is 125KHz sample rate @ 1MHz
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);

	ADMUX |= (1<<REFS0); // This sets ADC reference to AVCC

	// enable the ADC
	ADCSRA |= (1<<ADEN); // Enable ADC
}

uint16_t readAnalog(char channel)
{
	uint16_t adcVal;
	
	//ADMUX |= (channel & 0x0f);  //select input and ref
	
	ADMUX = (1<<REFS0) | (channel & 0x0f);  //select input and ref

	//ADMUX = 0x40; // Reset ADMUX to reset channel to zero

	//ADMUX |= channel; // set channel to MUX

	ADCSRA |= (1 << ADSC); // Start ADC Conversions

	// wait for conversion to complete
	while ((1 << ADSC) & ADCSRA)
	{
		// wait until end of conversion
	}

	adcVal = ADCL | (ADCH<<8);

	return adcVal; // return converted analog value
}
