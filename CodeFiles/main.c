#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "LEDComm.h"
#include "thermistors.h"
#include "voltageProbes.h"
#include "distanceSensor.h"
#include "stepperMotor.h"

/* Pin Layout */
#define VPROBEBATTPIN	0	// Voltage Probe Battery
#define VPROBERESPIN	1	// Voltage Probe Resistor

#define LEDLPIN		2	// Photoresistor Left
#define LEDMPIN		3	// Photoresistor Middle
#define LEDRPIN		4	// Photoresistor Right

#define THERM1PIN	5	// Thermistor 1
#define THERM2PIN	6	// Thermistor 2

#define DISTSENSORPIN	7	// IR Distance Sensor

#define RESETSUNANGLE	1
#define KEEPSUNANGLE	0

void calibrateLEDSMotor(Motor *motor, LEDS *leds)
{
	int i;
	int j;
	char *steps;
	char stepSize;
	int numbSteps;

	/* Reset step number in motor */
	motor->steps = 0;

	if((motor->mode == FULLSTEPMODE) || (motor->mode == WAVESTEPMODE))
	{
		steps = &motor->fullSteps;
		numbSteps = motor->fullStepNumb;
		stepSize = motor->fullStepsSize;
	}
	else // if motor->mode == HALFSTEPMODE
	{
		steps = &motor->halfSteps;
		numbSteps = motor->halfStepNumb;
		stepSize = motor->halfStepsSize;
	}

	/* Set motor direction */
	motor->direction = GOCLOCKWISE;
	motor->movementAmount = BYSTEP;

	/* Record offset values */
	while(motor->steps < numbSteps)
	{
		for(i=0; i<4; i++)
		{
			// read leds
			for(j=0; j<sizeof(leds->pins)/sizeof(char); j++)
			{
				leds->offset[motor->steps][j] 
					= readAnalog(leds->pins[j]);
			}
			motor->stepSeqIndex = i;
			moveMotor(motor);
			motor->steps = motor->steps + 1;
		}
	}

	motor->movementAmount = BYCYCLE;
	leds->direction = IDLE;
	motor->direction = IDLE;
}

void findSun(Motor *motor, LEDS *leds)
{
	char reorientationRequired = 0;
	int i;
	int numbSteps;

	if((motor->mode == FULLSTEPMODE) || (motor->mode == WAVESTEPMODE))
	{
		numbSteps = motor->fullStepNumb;
	}
	else // if motor->mode == HALFSTEPMODE
	{
		numbSteps = motor->halfStepNumb;
	}
	
	/* Keep Moving Motor Clockwise if the Middle Photoresistor Does Not Read SUNDETECTMULT Times the Max Deviation */
	if((leds->adcVal[leds->leftLED] < 
	    	SUNDETECTMULT*leds->offset[motor->steps][leds->leftLED]) &&
		(leds->adcVal[leds->middleLED] < 
		 	SUNDETECTMULT*leds->offset[motor->steps][leds->middleLED]) &&
		(leds->adcVal[leds->rightLED] < 
		 	SUNDETECTMULT*leds->offset[motor->steps][leds->rightLED]))
		{
			do
			{
				reorientationRequired = 1;
				motor->direction = GOCLOCKWISE;
				getLEDSVal(leds);
				moveMotor(motor);
			} while(leds->adcVal[leds->middleLED] < 
				SUNDETECTMULT*leds->offset[motor->steps][leds->middleLED]);
		}
	
	if(reorientationRequired)
	{
		motor->direction = IDLE;
		leds->direction = IDLE;
		motor->sunAngle = 0;
	}
}

int main()
{
	int i;
	char buffer[60] = {"\0"};

	uart_init();					// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init1(0x0F);// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	motor.movementAmount = BYCYCLE;
	
	/* ADC Setup */
	initADC();
	
	/* LED Setup */
	LEDS leds = leds_init(LEDLPIN, LEDMPIN, LEDRPIN);
	
	/* Thermistor Setup */
	Thermistor therm1 = therm1_init(THERM1PIN);
	Thermistor therm2 = therm2_init(THERM2PIN);

	/* Voltage Probes Setup */
	VoltageProbes voltageProbes = voltageProbes_init(VPROBEBATTPIN, VPROBERESPIN);
	
	/* Distance Sensor Setup */
	DistanceSensor distanceSensor = distanceSensor_init(DISTSENSORPIN);
	
	/* Calibrate LEDS and Motor */
	calibrateLEDSMotor(&motor, &leds);

	_delay_ms(500);

	/* Search for Sun */
	findSun(&motor, &leds);
	
	_delay_ms(250); 
	
	while(1)
	{
		sprintf(buffer, ""); 	// reset buffer
		
		/* Read Temperature Values */
		getTemp(&therm1); 		// get temp1
		getTemp(&therm2);		// get temp2
		
		/* Print Temperature Values */
		dtostrf(therm1.temp, 4, 3, buffer + strlen(buffer));	// convert double to string
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(therm2.temp, 5, 3, buffer + strlen(buffer));	// convert double to string
		//sprintf(buffer + strlen(buffer), " ");
		
		/* Read Voltage Values */
		readVoltageCurrent(&voltageProbes);
		
		/* Print Battery Voltage and Current */
		dtostrf(voltageProbes.voltage[voltageProbes.battVProbe], 
			6, 3, buffer + strlen(buffer));	// battery voltage (V)
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(voltageProbes.current, 5, 3, buffer + strlen(buffer));								// battery current (mA)
		sprintf(buffer + strlen(buffer), " ");
		
		/* Print Sun Angle */
		dtostrf(motor.sunAngle, 2, 1, buffer + strlen(buffer)); // degrees
		sprintf(buffer + strlen(buffer), " ");
		
		/* Read Distance */
		getDistance(&distanceSensor);
		
		/* Read LEDs */
		getLEDSVal(&leds);
		for(i=0;i<sizeof(leds.adcVal)/sizeof(uint16_t);i++)
		{
			if(PRINTPRVALS)
			{
				sprintf(buffer + strlen(buffer), 
					"%d ", leds.adcVal[i]);
			}
		}
		
		/* Check if Sun is Within Tracking Range and Search if it Is Lost */
		findSun(&motor, &leds);
		
		/* Move counter clockwise if left LED reads brighter light */
		if(leds.direction == GOCOUNTERCLOCKWISE)
		{
			motor.direction = GOCOUNTERCLOCKWISE; // move counter clockwise
			sprintf(buffer + strlen(buffer), "%d ", MOVINGCOUNTERCLOCK); 
		}
		
		/* Move clockwise if right LED reads brighter light */
		else if(leds.direction == GOCLOCKWISE)
		{
			motor.direction = GOCLOCKWISE;
			sprintf(buffer + strlen(buffer), "%d ", MOVINGCLOCK); 
		}
		
		/* Do nothing and idle position */
		else
		{
			motor.direction = IDLE;
			sprintf(buffer + strlen(buffer), "%d ", IDLING); 
		}
		
		if(distanceSensor.proximityWarning)
		{
			sprintf(buffer + strlen(buffer), "%d", PROXIMITYWARNING);
		}
		else
		{
			sprintf(buffer + strlen(buffer), "%d", NOOBJDETECTED);
		}
		
		/* Execute motor movement */
		moveMotor(&motor);
		
		/* Send data over serial */
		sprintf(buffer + strlen(buffer), "\n\r");
		write_uart(buffer);
	}

}
