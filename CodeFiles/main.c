#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "thermistors.h"
#include "stepperMotor.h"
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
	uint16_t leds[3];
	int ledChannels[] = {0, 1, 2};
	int leftLED = 0;
	int middleLED = 1;
	int rightLED = 2;
	
	/* Thermistor Setup */
	Thermistor therm1 = therm1_init(0);
	Thermistor therm2 = therm2_init(1);
	
	while(1)
	{
		sprintf(buffer, ""); // reset buffer
		
		getTemp(&therm1);
		//getTemp(&therm2);
		
		dtostrf(therm1.temp, 5, 3, buffer + strlen(buffer));
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(therm2.temp, 5, 3, buffer + strlen(buffer));
		
		//sprintf(buffer + strlen(buffer), "%d %d ", therm1.adcVal, therm2.adcVal);
		
		//sprintf(buffer + strlen(buffer), "%d %d ", therm1.temp, therm2.temp);
		
		//// read LEDs
		//for(i=0; i<sizeof(leds)/sizeof(int); i++)
		//{
			//leds[i] = readAnalog(ledChannels[i]);
			//sprintf(buffer + strlen(buffer), "%d ", leds[i]); 
		//}
		//
		//// move counter clockwise if left LED reads brighter light
		//if((leds[leftLED] > leds[middleLED]) && (leds[leftLED] > leds[rightLED]))
		//{
			//motor.clock = 0; // move counter clockwise
			//moveMotor(&motor); // run through one cycle
			//sprintf(buffer + strlen(buffer), "Moving left."); 
		//}
		//
		//// move clockwise if right LED reads brighter light
		//else if((leds[rightLED] > leds[middleLED]) && (leds[rightLED] > leds[leftLED]))
		//{
			//motor.clock = 1;
			//moveMotor(&motor); // run through one cycle
			//sprintf(buffer + strlen(buffer), "Moving right."); 
		//}
		//else
		//{
			//sprintf(buffer + strlen(buffer), "Idling."); 
		//}
		//
		sprintf(buffer + strlen(buffer), "\n\r");
		write_uart(buffer);
	}

}
