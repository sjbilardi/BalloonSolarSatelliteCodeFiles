#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "LEDComm.h"
#include "thermistors.h"
#include "voltageProbes.h"
#include "stepperMotor.h"
//#include "allInterrupts.h"

/* Pin Layout */
#define LEDLPIN			0	// Photoresistor Left
#define LEDMPIN			1	// Photoresistor Middle
#define LEDRPIN			2	// Photoresistor Right

#define THERM1PIN		3	// Thermistor 1
#define THERM2PIN		4	// Thermistor 2

#define VPROBEBATTPIN	5	// Voltage Probe Battery
#define VPROBERESPIN	6	// Voltage Probe Resistor

int main()
{
	int i;
	char buffer[50] = {"\0"};

	uart_init();					// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init(0x0F);	// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	
	/* ADC Setup */
	initADC();
	
	/* LED Setup */
	LEDS leds = leds_init(LEDLPIN, LEDMPIN, LEDRPIN);
	
	/* Thermistor Setup */
	Thermistor therm1 = therm1_init(THERM1PIN);
	Thermistor therm2 = therm2_init(THERM2PIN);

	/* Voltage Probes Setup */
	VoltageProbes voltageProbes = voltageProbes_init(VPROBEBATTPIN, VPROBERESPIN);
	
	while(1)
	{
		sprintf(buffer, ""); 	// reset buffer
		
		getTemp(&therm1); 		// get temp1
		getTemp(&therm2);		// get temp2
		
		/* Print Temperature Values */
		dtostrf(therm1.temp, 5, 3, buffer + strlen(buffer));	// convert double to string
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(therm2.temp, 5, 3, buffer + strlen(buffer));	// convert double to string
		sprintf(buffer + strlen(buffer), " ");
		
		/* Read LEDs */
		getLEDSVal(&leds);
		for(i=0;i<sizeof(leds.adcVal)/sizeof(uint16_t);i++)
		{
			sprintf(buffer + strlen(buffer), "%d ", leds.adcVal[i]);
		}
		
		/* Move counter clockwise if left LED reads brighter light */
		if(leds.direction == GOCOUNTERCLOCKWISE)
		{
			motor.direction = GOCOUNTERCLOCKWISE; // move counter clockwise
			sprintf(buffer + strlen(buffer), "Moving left."); 
		}
		
		/* Move clockwise if right LED reads brighter light */
		else if(leds.direction == GOCLOCKWISE)
		{
			motor.direction = GOCLOCKWISE;
			sprintf(buffer + strlen(buffer), "Moving right."); 
		}
		
		/* Do nothing and idle position */
		else
		{
			motor.direction = IDLE;
			sprintf(buffer + strlen(buffer), "Idling."); 
		}
		
		/* Execute motor movement */
		moveMotor(&motor);
		
		/* Send data over serial */
		sprintf(buffer + strlen(buffer), "\n\r");

		//sprintf(buffer, ""); 	// reset buffer
		//sprintf(buffer + strlen(buffer), "This is a test.\n\r");
		write_uart(buffer);
	}

}
