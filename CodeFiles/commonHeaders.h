#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#define VCC 5.1 // V

/* Options for satellite direction movement */
#define GOCOUNTERCLOCKWISE  0
#define IDLE				1
#define GOCLOCKWISE			2

/* Motor step options */
#define WAVESTEPMODE 0 // waveStepMode
#define FULLSTEPMODE 1 // fullStepMode
#define HALFSTEPMODE 2 // halfStepMode
#define BYCYCLE		 3 // Move one cycle when during function call
#define BYSTEP		 4 // Move one step at a time
#define BYDEGREES	 5 // Move number of degrees during function call

#define MOTORDELAY	30 // ms

#define SUNDETECTMULT	1.3	// sun detection multiplier for max deviation

/* Comment codes */
#define MOVINGCLOCK         0 // sun detected on right led; moving clockwise
#define MOVINGCOUNTERCLOCK  1 // sun detected on left  led; moving counterclockwise
#define IDLING              2 // satellite oriented towards sun; idling position
#define BATTERYLOW          3 // battery voltage low; reaching specified limit
#define SEEKINGSUN          4 // searching for sun
#define PROXIMITYWARNING	5 // object detected within ~1 meter
#define NOOBJDETECTED		6 // no object detected; path ahead of spacecraft clear

/* Enable serial outputs */ 
#define DEBUG 0
#define PRINTPRVALS 0
