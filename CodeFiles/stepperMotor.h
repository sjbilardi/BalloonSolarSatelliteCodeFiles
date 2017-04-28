#include "commonHeaders.h"

/* Motor */
typedef struct {
	int waveSteps[4];
	int waveStepNumb;
	double waveStepRes;

	int fullSteps[4];
	int fullStepNumb;
	double fullStepRes;

	int halfSteps[8];
	int halfStepNumb;
	double halfStepRes;
	
	char waveStepsSize;
	char fullStepsSize;
	char halfStepsSize;
	
	char mode;
	char direction;
	char movementAmount;
	double moveDegrees;
	double sunAngle;
	int steps;
	char stepSeqIndex;
	
	char pins;
	} Motor;

/* Motor Functions */
Motor motor_init1(char pins)
{
	Motor motor = {	{0x02, 0x08, 0x04, 0x01},							// waveSteps
					200,												// steps
					1.8,												// degree
					{0x03, 0x0A, 0x0C, 0x05},							// fullSteps
					200,												// steps
					1.8,												// degrees
					{0x03, 0x02, 0x0A, 0x08, 0x0C, 0x04, 0x05, 0x01},	// halfSteps
					400, 												// steps
					0.9,												// degrees
					4, // waveStepsSize
					4, // fullStepsSize
					8, // halfStepsSize
					FULLSTEPMODE, // mode (default = fullStepMode)
					IDLE, // Idle position
					BYCYCLE,
					0.0,
					0.0,
					0,
					0,
					pins}; // PORTB pins 0-3
	DDRB |= pins; // set register B pins to high; configured as high
	return motor;
}

/* Motor Functions */
Motor motor_init4(char pins)
{
	Motor motor = {	{0x08, 0x02, 0x04, 0x01},							// waveSteps
					200,												// steps
					1.8,												// degree
					{0x0A, 0x06, 0x05, 0x09},							// fullSteps
					200,												// steps
					1.8,												// degrees
					{0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x01, 0x09},	// halfSteps
					400, 												// steps
					0.9,												// degrees
					4, // waveStepsSize
					4, // fullStepsSize
					8, // halfStepsSize
					FULLSTEPMODE, // mode (default = fullStepMode)
					IDLE, // Idle position
					BYCYCLE,
					0.0,
					0.0,
					0,
					0,
					pins}; // PORTB pins 0-3
	DDRB |= pins; // set register B pins to high; configured as high
	return motor;
}

void moveMotor(Motor *motor)
{
	/* 
		Move motor one cycle in direction determined by
		position of sun.
	*/
	int i = 0;
	int *steps; 
	char numbStep = 0;
	double res = 0.0;
	int stepNumb = 0;
	
	switch(motor->mode)
	{
		case WAVESTEPMODE:
			steps = &motor->waveSteps;
			numbStep = motor->waveStepsSize; 
			res = motor->waveStepRes;
			stepNumb = motor->waveStepNumb;
			break;
		case FULLSTEPMODE:
			steps = &motor->fullSteps;
			numbStep = motor->fullStepsSize; 
			res = motor->fullStepRes;
			stepNumb = motor->fullStepNumb;
			break;
		case HALFSTEPMODE:
			steps = &motor->halfSteps;
			numbStep = motor->halfStepsSize;
			res = motor->halfStepRes;
			stepNumb = motor->halfStepNumb;
			break;
	}
	
	if(motor->direction == GOCLOCKWISE)
	{	
		if(motor->movementAmount == BYCYCLE)
		{
			for(i=0; i<numbStep; i++)
			{
				PORTB &= 0xF0; 		// Turn off the stepper motor only
				PORTB |= *(steps + i); 	// Cause the stepper motor to make a step
				_delay_ms(MOTORDELAY); 	// Delay the time between the step and shutting off the motor
				
				/* Increment sun angle */
				motor->sunAngle = motor->sunAngle + res;
				motor->steps = motor->steps + 1;
				
				if(motor->steps > stepNumb - 1)
				{
					motor->steps = motor->steps - stepNumb;
				}
				
				if(motor->sunAngle > 360.0 - res)
				{
					motor->sunAngle = motor->sunAngle - 360;	
				}
			}
		}
		
		else if(motor->movementAmount == BYSTEP)
		{
			PORTB &= 0xF0; 					// Turn off the stepper motor only
			PORTB |= *(steps + motor->stepSeqIndex); 	// Cause the stepper motor to make a step
			_delay_ms(MOTORDELAY); 				// Delay the time between the step and shutting off the motor
		}

		else
		{
			// do nothing
		}
	}
	else if(motor->direction == GOCOUNTERCLOCKWISE)
	{
		if(motor->movementAmount == BYCYCLE)
		{
			for(i=numbStep-1; i>=0; i--)
			{
				PORTB &= 0xF0; 		// Turn off the stepper motor only
				PORTB |= *(steps + i); 	// Cause the stepper motor to make a step
				_delay_ms(MOTORDELAY); 	// Delay the time between the step and shutting off the motor
				
				/* Decrement sun angle */
				motor->sunAngle = motor->sunAngle - res;
				motor->steps = motor->steps - 1;
				
				if(motor->steps < 0)
				{
					motor->steps = motor->steps + stepNumb;
				}
				
				if(motor->sunAngle < 0)
				{
					motor->sunAngle = motor->sunAngle + 360;
				}
			}
		}

		else if(motor->movementAmount == BYSTEP)
		{
			PORTB &= 0xF0; 					// Turn off the stepper motor only
			PORTB |= *(steps + motor->stepSeqIndex); 	// Cause the stepper motor to make a step
			_delay_ms(MOTORDELAY); 				// Delay the time between the step and shutting off the motor
		}

		else
		{
			// do nothing
		}
	}
	else
	{
		// Idle
	}
}
