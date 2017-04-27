# Solar Tracking and Autonomous Bus Microcontroller Code Files
The project will allow a mock satellite to:

- track a light source in one dimension using a stepper motor and LED's.
- measure the temperature using two seperate thermistors.
- measure bus battery voltage.
- measure bus battery current.
- transmit telemetry to ground station.

## Updates:

### 2017-04-27

- Updated code files to final version.
- New features:
  + Uses IR sensor to determine if an object is within 90cm of bus.
  + Rotates 360 degrees at beginning to measure noise level  environment. Sets detection thresholds.
  + Searches for sun before and entering and during main loop. Resets sun angle when sun is detected. 

### 2017-04-23

- "CodeFiles" directory contains "main.c" file which includes the .h files in this directory.
- Header files exist for each component of satellite and have been updated to use structures, containing specifications for these components.
- Added support for thermistors with conversion to degrees Celsius.

### 2017-04-10

- Added "main.c" to repository.
- "main.c" contains functions for one-way serial communication and basic stepper motor control (one directional movement only).
- Made header files for each major component of the satellite. Headers files contain the functions for components such as stepper motors, UARTs and ADC's.
