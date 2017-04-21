#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#define F_CPU 1000000UL
#include <util/delay.h>

/* Options for satellite direction movement */
#define GOCOUNTERCLOCK  0
#define IDLE            1
#define GOCLOCKWISE     2

/* Comment codes */
#define MOVINGCLOCK         0 // sun detected on right led; moving clockwise
#define MOVINGCOUNTERCLOCK  1 // sun detected on left  led; moving counterclowise
#define IDLING              2 // satellite oriented towards sun; idling position
#define BATTERYLOW          3 // battery voltage low; reaching specified limit
#define 

/* Enable debug serial outputs */ 
#define DEBUG 0
