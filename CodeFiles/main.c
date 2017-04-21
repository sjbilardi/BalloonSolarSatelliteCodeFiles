#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "thermistors.h"
#include "stepperMotor.h"
#include "LEDComm.h"
#include "allInterrupts.h"

int main()
{
	int i;
	char buffer[40] = {"\0"};

	uart_init();				// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init(0x0F);		// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	
	/* ADC Setup */
	initADC();
	
	/* LED Setup */
	LEDS leds = leds_init(0, 1, 2);
	
	/* Thermistor Setup */
	Thermistor therm1 = therm1_init(3);
	Thermistor therm2 = therm2_init(4);
	
	while(1)
	{
		sprintf(buffer, ""); 	// reset buffer
		
		getTemp(&therm1); 	// get temp1
		getTemp(&therm2);	// get temp2
		
		/* Print Temperature Values */
		dtostrf(therm1.temp, 5, 3, buffer + strlen(buffer));	// convert double to string
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(therm2.temp, 5, 3, buffer + strlen(buffer));	// convert double to string
		
		/* Read LEDs */
		getLEDSVal(&leds);
		
		/* Move counter clockwise if left LED reads brighter light */
		if(leds.direction == GOCOUNTERCLOCKWISE)
		{
			motor.clock = 0; // move counter clockwise
			moveMotor(&motor); // run through one cycle
			sprintf(buffer + strlen(buffer), "Moving left."); 
		}
		
		/* Move clockwise if right LED reads brighter light */
		else if(leds.direction == GOCLOCKWISE)
		{
			motor.clock = 1;
			moveMotor(&motor); // run through one cycle
			sprintf(buffer + strlen(buffer), "Moving right."); 
		}
		
		/* Do nothing and idle position */
		else
		{
			sprintf(buffer + strlen(buffer), "Idling."); 
		}
		
		sprintf(buffer + strlen(buffer), "\n\r");
		write_uart(buffer);
	}

}
