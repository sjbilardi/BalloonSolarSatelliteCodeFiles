#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "stepperMotor.h"
#include "allInterrupts.h"

int main()
{
	char buffer[20];

	uart_init();				// initialize the UART for serial communication with computer
	
	/* Motor Setup */
	Motor motor = motor_init(0x0F);		// initialize motor to pins 0-3 on PORTB
	motor.mode = FULLSTEPMODE;		// set motor to move using full step pattern
	
	while(1)
	{
		moveMotor(&motor); // run through one cycle
	}

}
