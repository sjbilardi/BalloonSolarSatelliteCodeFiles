#include "commonHeaders.h"

typedef struct {
	uint16_t adcVal;
	double distance;
	char proximityWarning;
	char pin;
} DistanceSensor;

DistanceSensor distanceSensor_init(char pin)
{
	DistanceSensor distanceSensor = {	
		0,
		0.0,	// cm
		0,
		pin};
	
	return distanceSensor;
}

void getDistance(DistanceSensor *distanceSensor)
{
	distanceSensor->adcVal = readAnalog(distanceSensor->pin);
	
	distanceSensor->distance = 9801.8*pow(((double)distanceSensor->adcVal), 
		-1.127); // use power function to calibrate ADC to distance (cm)
	
	if(distanceSensor->distance < 90.0)
	{
		distanceSensor->proximityWarning = 1;
	}
	
	else
	{
		distanceSensor->proximityWarning = 0;
	}
}
