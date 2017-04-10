#include "commonHeaders.h"

double calcDistance(uint16_t IRVal)
{
	double Val = (double)IRVal; // make double version of IRVal
	double distance;

	distance = 9801.8*pow(Val, -1.127); // use power function to calibrate ADC to distance (cm)
	
	return distance;
}