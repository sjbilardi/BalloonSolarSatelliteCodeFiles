#include "commonHeaders.h"
#include "serialComm.h"
#include "adcComm.h"
#include "LEDComm.h"
#include "thermistors.h"
#include "voltageProbes.h"
#include "stepperMotor.h"
//#include "allInterrupts.h"

/* Pin Layout */
#define VPROBEBATTPIN	0	// Voltage Probe Battery
#define VPROBERESPIN	1	// Voltage Probe Resistor

#define LEDLPIN			2	// Photoresistor Left
#define LEDMPIN			3	// Photoresistor Middle
#define LEDRPIN			4	// Photoresistor Right

#define THERM1PIN		5	// Thermistor 1
#define THERM2PIN		6	// Thermistor 2

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
		dtostrf(voltageProbes.voltage[voltageProbes.battVProbe], 6, 3, buffer + strlen(buffer));	// battery voltage (V)
		sprintf(buffer + strlen(buffer), " ");
		dtostrf(voltageProbes.current, 5, 3, buffer + strlen(buffer));								// battery current (mA)
		sprintf(buffer + strlen(buffer), " ");
		
		/* Print Sun Angle */
		dtostrf(motor.sunAngle, 2, 1, buffer + strlen(buffer)); // degrees
		sprintf(buffer + strlen(buffer), " ");
		
		/* Read LEDs */
		getLEDSVal(&leds);
		for(i=0;i<sizeof(leds.adcVal)/sizeof(uint16_t);i++)
		{
			if(PRINTPRVALS)
			{
				sprintf(buffer + strlen(buffer), "%d ", leds.adcVal[i]);
			}
		}
		
		/* Move counter clockwise if left LED reads brighter light */
		if(leds.direction == GOCOUNTERCLOCKWISE)
		{
			motor.direction = GOCOUNTERCLOCKWISE; // move counter clockwise
			sprintf(buffer + strlen(buffer), "%d", MOVINGCOUNTERCLOCK); 
		}
		
		/* Move clockwise if right LED reads brighter light */
		else if(leds.direction == GOCLOCKWISE)
		{
			motor.direction = GOCLOCKWISE;
			sprintf(buffer + strlen(buffer), "%d", MOVINGCLOCK); 
		}
		
		/* Do nothing and idle position */
		else
		{
			motor.direction = IDLE;
			sprintf(buffer + strlen(buffer), "%d", IDLING); 
		}
		
		/* Execute motor movement */
		//motor.direction = GOCLOCKWISE;
		moveMotor(&motor);
		
		//sprintf(buffer + strlen(buffer), " ");
		//sprintf(buffer + strlen(buffer), "%d", motor.steps);
		
		/* Send data over serial */
		sprintf(buffer + strlen(buffer), "\n\r");
		write_uart(buffer);
	}

}
