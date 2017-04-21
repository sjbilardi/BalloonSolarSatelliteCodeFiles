#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#define F_CPU 1000000UL
#include <util/delay.h>

/* Determine which direction system should move */
#define GOCOUNTERCLOCK  0
#define IDLE            1
#define GOCLOCKWISE     2

#define DEBUG 0
