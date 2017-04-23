# Solar Tracking and Autonomous Bus Microcontroller Code Files
The project will allow a mock satellite to:

- track a light source in one dimension using a stepper motor and LED's.
- measure the temperature using two seperate thermistors.
- measure bus battery voltage.
- measure bus battery current.
- transmit telemetry to ground station.

## Updates:

### 2017-04-10

- Added "main.c" to repository.
- "main.c" contains functions for one-way serial communication and basic stepper motor control (one directional movement only).
- Made header files for each major component of the satellite. Headers files contain the functions for components such as stepper motors, UARTs and ADC's.

### 2017-04-23

- "CodeFiles" directory contains "main.c" file which includes the .h files in this directory.
- Header files exist for each component of satellite and have been updated to use structures, containing specifications for these components.
- Added support for thermistors with conversion to degrees Celsius.
