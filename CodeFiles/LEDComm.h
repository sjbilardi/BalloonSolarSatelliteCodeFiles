#include "commonHeaders.h"

/* LED */
typedef struct {
	uint16_t adcVal[3];
  
  	char leftLED;
  	char middleLED;
  	char rightLED;
	
	char direction;
	
	char pins[3];

	uint16_t offset[3];
	uint16_t offsetAvg;
	uint16_t deviation[3];
	char maxDeviationIndex;
	} LEDS;

/* LEDS Functions */
LEDS leds_init(char leftLEDpin, char middleLEDpin, char rightLEDpin)
{
	int i;
	LEDS leds = {	{0, 0, 0},  // default val to zero
		0, // left LED index
		1, // middle LED index
        2, // right LED index
		IDLE,
        {leftLEDpin, middleLEDpin, rightLEDpin},
		{0, 0, 0},
		0,
		{0, 0, 0},
		0};

	getMaxDeviation(&leds);

	return leds;
}

void getMaxDeviation(LEDS *leds)
{
	char i;
	
	/* Read Photoresitor ADC Noise Values */
	for(i=0; i<sizeof(leds->offset)/sizeof(uint16_t);i++)
	{
		leds->offset[i] = readAnalog(leds->pins[i]);
		leds->offsetAvg = leds->offset[i]; // sum offset values
	}
	
	leds->offsetAvg = leds->offsetAvg / (sizeof(leds->offset)/sizeof(uint16_t)); // get average of offset values
	
	/* Find Maximum Deviation from Offset Average */
	for(i=0; i<sizeof(leds->offset)/sizeof(uint16_t);i++)
	{
		leds->deviation[i] = abs(leds->offsetAvg - leds->offset[i]);
		if(i > 0)
		{
			if(leds->deviation[i] > leds->deviation[i-1])
			{
				leds->maxDeviationIndex = i;
			}
		}
	}
}

void getLEDSVal(LEDS *leds)
{
	char i;
	
	/* Read Photoresistor ADC Values and Correct for Offset */
	for(i=0; i<sizeof(leds->adcVal)/sizeof(uint16_t);i++)
	{
		leds->adcVal[i] = readAnalog(leds->pins[i]);
		//leds->adcVal[i] = leds->adcVal[i] - leds->offset[i];
	}
	
	/* Move counter clockwise if left photoresitor reads brighter light */
	if((leds->adcVal[leds->leftLED] > leds->adcVal[leds->middleLED]) && (leds->adcVal[leds->leftLED] > leds->adcVal[leds->rightLED]))
	{
		leds->direction = GOCOUNTERCLOCKWISE;
	}
		
	/* Move clockwise if right photoresitor reads brighter light */
	else if((leds->adcVal[leds->rightLED] > leds->adcVal[leds->middleLED]) && (leds->adcVal[leds->rightLED] > leds->adcVal[leds->leftLED]))
	{
		leds->direction = GOCLOCKWISE;
	}
		
	/* Do nothing and idle position */
	else
	{
		leds->direction = IDLE;
	}
}
