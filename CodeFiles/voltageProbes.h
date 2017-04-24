#include "commonHeaders.h"

typedef struct {
	uint16_t adcVal;
	double voltage;
	double R;
	double RL;
	char pins;
} VoltageProbe;

VoltageProbe voltageProbe_init(char pins)
{
	VoltageProbe voltageProbe{	0,
								0.0,
								0,
								0,
								pins};
	return voltageProbe;
}

void readVoltage(VoltageProbe *voltageProbe)
{
	
}