#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "stepperMotor.h"
#include "allInterrupts.h"

int main()
{
	int i;
	char buffer[30];

	uart_init();				// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init(0x0F);		// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	
	/* ADC Setup */
	initADC();
	uint16_t leds[3];
	int ledChannels[] = {0, 1, 2};
	int leftLED = 0;
	int middleLED = 1;
	int rightLED = 2;
	
	while(1)
	{
		sprintf(buffer, "LED Values: ");
		for(i=0; i<sizeof(leds)/sizeof(int); i++)
		{
			leds[i] = readAnalog(ledChannels[i]);
			sprintf(buffer + strlen(buffer), "%d, ", leds[i]); 
		}
		
		if((leds[leftLED] > leds[middleLED]) && (leds[leftLED] > leds[rightLED]))
		{
			motor.clock = 0; // move counter clockwise
			moveMotor(&motor); // run through one cycle
			write_uart("Moving left.\n\r");
		}
		else if((leds[rightLED] > leds[middleLED]) && (leds[rightLED] > leds[leftLED]))
		{
			motor.clock = 1;
			moveMotor(&motor); // run through one cycle
			write_uart("Moving right.\n\r");
		}
		
		sprintf(buffer + strlen(buffer), "\n\r"); 
		write_uart(buffer);
	}

}
