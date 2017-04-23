#include "commonHeaders.h"

/* LED */
typedef struct {
	uint16_t val[3];
  
  	char leftLED;
  	char middleLED;
  	char rightLED;
	
	char direction;
	
	char pins[3];
	} LEDS;

/* LEDS Functions */
LEDS leds_init(char leftLEDpin, char middleLEDpin, char rightLEDpin)
{
	LEDS leds = {	{0, 0, 0},  // default val to zero
		0, // left LED index
		1, // middle LED index
        2, // right LED index
		IDLE,
        {leftLEDpin, middleLEDpin, rightLEDpin}}; // 
	return leds;
}

void getLEDSVal(LEDS *leds)
{
	char i;
	
	/* Read LED ADC Values */
	for(i=0; i<sizeof(leds->val)/sizeof(uint16_t);i++)
	{
		leds->val[i] = readAnalog(leds->pins[i]);
	}
	
	/* Move counter clockwise if left LED reads brighter light */
	if((leds->val[leds->leftLED] > leds->val[leds->middleLED]) && (leds->val[leds->leftLED] > leds->val[leds->rightLED]))
	{
		leds->direction = GOCOUNTERCLOCKWISE;
	}
		
	/* Move clockwise if right LED reads brighter light */
	else if((leds->val[leds->rightLED] > leds->val[leds->middleLED]) && (leds->val[leds->rightLED] > leds->val[leds->leftLED]))
	{
		leds->direction = GOCLOCKWISE;
	}
		
	/* Do nothing and idle position */
	else
	{
		leds->direction = IDLE;
	}
}
