#include "commonHeaders.h"

typedef struct {
	uint16_t adcVal[2];
	double voltage[2];
	double current;
	double vcc;
	double R;
	double RL;
	double currentResistor;
	char battVProbe;
	char ResVProbe;
	char pin[2];
} VoltageProbes;

VoltageProbes voltageProbes_init(char pin1, char pin2)
{
	VoltageProbes voltageProbes = {	{0, 0},
									{0.0, 0.0}, // V
									0.0, // mA
									5.0, // V
									5.1, // k-ohms
									5.1, // k-ohms
									2,	 // ohms
									0,
									1,
									{pin1, pin2}};
	return voltageProbes;
}

void readVoltageCurrent(VoltageProbes *voltageProbes)
{
	char i;

	/* Read ADC values from probes and convert to voltages */
	for(i=0; i<sizeof(voltageProbes->adcVal)/sizeof(uint16_t); i++)
	{
		voltageProbes->adcVal[i] = readAnalog(voltageProbes->pin[i]);
		voltageProbes->voltage[i] = (((double)voltageProbes->adcVal[i]/1024)) * voltageProbes->vcc * 
									(voltageProbes->R + voltageProbes->RL)/voltageProbes->R; // V
	}

	/* Calculate the current using the voltage values */
	voltageProbes->current = fabs(voltageProbes->voltage[0] - voltageProbes->voltage[1]) / 
							 voltageProbes->currentResistor * 1000; // mA
}