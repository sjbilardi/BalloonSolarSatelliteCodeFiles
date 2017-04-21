#include "commonHeaders.h"

/* LED */
typedef struct {
	uint16_t val[3];
  
  char leftLED;
  char middleLED;
  char rightLED;
	
	char pins[3];
	} LEDS;

/* Motor Functions */
LEDS leds_init(char rightLEDpin, char middleLEDpin, char rightLEDpin)
{
	LEDS leds = {	{0, 0, 0},  // default val to zero
					0, // left LED index
					1, // middle LED index
          2, // right LED index
          {rightLEDpin, middleLEDpin, rightLEDpin}}; // 
	return leds;
}
