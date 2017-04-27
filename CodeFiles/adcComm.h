#include "commonHeaders.h"

void initADC()
{
	// initializes ADC for 10-bit reading

	// Set ADC prescalar to 8: Which is 125KHz sample rate @ 1MHz
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);

	ADMUX |= (1<<REFS0); // This sets ADC reference to AVCC

	ADCSRA |= (1<<ADEN); // Enable ADC
}

uint16_t readAnalog(char channel)
{
	uint16_t adcVal;
	
	ADMUX = (1<<REFS0) | (channel & 0x0f);  //select input and ref

	ADCSRA |= (1 << ADSC); // Start ADC Conversions

	// wait for conversion to complete
	while ((1 << ADSC) & ADCSRA)
	{
		// wait until end of conversion
	}

	adcVal = ADCL | (ADCH<<8);

	return adcVal; // return converted analog value
}
