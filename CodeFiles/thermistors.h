#include "commonHeaders.h"

/* Thermistors */
typedef struct {
	int thermNumb;
	
	double Ro;
	double To;
	double B;
	
	double Vcc;
	double Vrange;
	double R;
	double Rt;
	
	uint16_t adcVal;
	double Vo;
	double temp;
	
	char pin;
} Thermistor;

/* Motor Functions */
Thermistor therm1_init(char pin)
{
	Thermistor therm1 = {	1,
							9.88,		// k-ohm
							297.5,		// K
							4296.71,
							5.0,
							5.0,
							12.3231,	// k-ohm
							0,
							0,
							0.0,
							pin	};
return therm1;
}

Thermistor therm2_init(char pin)
{
	Thermistor therm2 = {	2,
							9.83,		// k-ohm
							297.5,		// K
							4241.48,
							5.0,
							5.0,
							12.2836,	// k-ohm
							0,
							0,
							0.0,
							pin	};
	return therm2;
}

void getTemp(Thermistor *therm)
{
	/* Converts the ADC value measured from the port with a thermistor connected
	   and converts the value to degrees Celsius. */
	
	char buffer[60];
	sprintf(buffer, "");
	
	// read ADC value
	therm->adcVal = readAnalog(therm->pin);
	
	therm->Vo = ((double)therm->adcVal)/pow(2,10) * therm->Vrange;
	
	therm->Rt = therm->R*(therm->Vcc/therm->Vo - 1); 
	
	therm->temp = pow((log(therm->Rt/therm->Ro)/therm->B + 1/therm->To), -1) - 273.15;
	
	if(DEBUG == 1)
	{
		sprintf(buffer + strlen(buffer), "ADC: %d ", therm->adcVal);
	
		sprintf(buffer + strlen(buffer), "V: ");
		dtostrf(therm->Vo, 4, 3, buffer + strlen(buffer));
	
		sprintf(buffer + strlen(buffer), " Rt: ");
		dtostrf(therm->Rt, 5, 3, buffer + strlen(buffer));
	
		sprintf(buffer + strlen(buffer), " T: ");
		dtostrf(therm->temp, 5, 3, buffer + strlen(buffer));
	
		sprintf(buffer + strlen(buffer), "\n\r");
		write_uart(buffer);
	}
}
