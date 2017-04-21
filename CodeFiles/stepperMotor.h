#include "commonHeaders.h"

/* Motor */
typedef struct {
	int waveSteps[4];
	int fullSteps[4];
	int halfSteps[8];
	
	int waveStepsSize;
	int fullStepsSize;
	int halfStepsSize;
	
	int mode;
	int direction;
	
	int pins;
	} Motor;

/* Motor Functions */
Motor motor_init(int pins)
{
	Motor motor = {	{0x08, 0x02, 0x04, 0x01},							// waveSteps
					{0x0A, 0x06, 0x05, 0x09},							// fullSteps
					{0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x01, 0x09},	// halfSteps
					4, // waveStepsSize
					4, // fullStepsSize
					8, // halfStepsSize
					FULLSTEPMODE, // mode (default = fullStepMode)
					1, // enable clockwise movement
					pins}; // PORTB pins 0-3
	DDRB |= pins; // set register B pins to high; configured as high
	return motor;
}

void moveMotor(Motor *motor)
{
	int i = 0;
	int *steps; 
	int numbStep;
	
	switch(motor->mode)
	{
		case WAVESTEPMODE:
			steps = &motor->waveSteps;
			numbStep = motor->waveStepsSize; 
			break;
		case FULLSTEPMODE:
			steps = &motor->fullSteps;
			numbStep = motor->fullStepsSize; 
			break;
		case HALFSTEPMODE:
			steps = &motor->halfSteps;
			numbStep = motor->halfStepsSize;
			break;
	}
	
	if(motor->direction == GOCLOCKWISE)
	{
		for(i=0; i<numbStep; i++)
		{
			PORTB &= 0xF0; // Turn off the stepper motor only
			_delay_ms(50); // Delay the time between turning off and on the motor
			PORTB |= *(steps + i); // Cause the stepper motor to make a step
			_delay_ms(50); // Delay the time between the step and shutting off the motor
		}
	}
	else if(motor->direction == GOCOUNTERCLOCKWISE)
	{
		for(i=numbStep-1; i>=0; i--)
		{
			PORTB &= 0xF0; // Turn off the stepper motor only
			_delay_ms(50); // Delay the time between turning off and on the motor
			PORTB |= *(steps + i); // Cause the stepper motor to make a step
			_delay_ms(50); // Delay the time between the step and shutting off the motor
		}
	}
	else
	{
		// Idle
	}
}
