#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "thermistors.h"
#include "stepperMotor.h"
#include "LEDComm.h"
//#include "allInterrupts.h"

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
	LEDS leds = leds_init(0, 1, 2);
	
	/* Thermistor Setup */
	Thermistor therm1 = therm1_init(3);
	Thermistor therm2 = therm2_init(4);
	
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
		for(i=0;i<sizeof(leds.val)/sizeof(uint16_t);i++)
		{
			sprintf(buffer + strlen(buffer), "%d ", leds.val[i]);
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
		write_uart(buffer);
	}

}
