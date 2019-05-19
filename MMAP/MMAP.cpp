// Do not remove the include below

#include "MMAP.h"

#include <HardwareSerial.h>
#include <Wire.h>

#include "compass/MPU9250COMPASS.h"
#include "graphs/MAINGRAPH.h"
#include "hid/MainButtonsLogic.h"
// #include "motordriver/L298NMotorDriver.h"
#include "motordriver/VNH2SP30MotorDriver.h"


// Globals

#include "Globals.h"
#if DEBUG == 1
long lastDisplayHeading = millis();
#define DEBUG_DISPLAY_HEADING_INTERVAL 2000
long lastDisplayCommand = millis();
#define DEBUG_DISPLAY_COMMAND_INTERVAL 2000
int frameCount = 0;
long sampleRateDisplay = millis();
#endif

#define SERIAL_BITRATE 115200
short state = 0; // This variable holds the current state of the system

// COMPASS

COMPASS* compass;
float heading; // Current heading calculated from compass

// AUTOPILOT

float bearing = 0;
short command = 0;
short deadband = 10;

// PID Variables
float kp = 1000;
float kd = 0;
long previousTime;
long deltaTime;
float headingError;
float previousError;
float deltaError;

// Graphs
MAINGRAPHDATA mainGraphData;
MAINGRAPH mainGraphDisplay;

// HDI
MainButtonsLogic mainButtonsLogic(state, heading, bearing);

// MotorDriver
MotorDriver* motorDriver;

// Current
float maxCurrent = 0;

// Sensibilidad del sensor en V/A
float SENSIBILITY = 0.13;


//The setup function is called once at startup of the sketch
void setup() {
#if DEBUG > 0
	Serial.begin(SERIAL_BITRATE);
#endif
	Wire.begin();
	Wire.setClock(400000L);
	compass = new MPU9250COMPASS();
	// motorDriver = new L298NMotorDriver(28, 29);
	motorDriver = new VNH2SP30MotorDriver(5, 6, 4);

	setupButtons(&mainButtonsLogic);

	mainGraphData.heading = &heading;
	mainGraphData.state = &state;
	mainGraphData.bearing = &bearing;
	mainGraphData.actualCommand = &motorDriver->actualCommand;
	mainGraphDisplay.setupDisplay();

	// Setup Current measurement
	pinMode(A0, INPUT);
}

// The loop function is called in an endless loop
void loop() {
#if DEBUG == 1
	frameCount++;
#endif

	// Process button logic
	mainButtonsLogic.tick();


	// Get heading from compass
	compass->getHeading(heading);

	float current = getCorriente(3);

	if (maxCurrent < abs(current))
		maxCurrent = abs(current);

	if (abs(current) > 6){
		Serial.print("OVERLOAD: "); Serial.print(abs(current), 2); Serial.println(" A");
		motorDriver->correctTo(0);
		state = 0;

	}


#if DEBUG == 1
	if (millis() - lastDisplayHeading > DEBUG_DISPLAY_HEADING_INTERVAL) {
		Serial.print("Heading: ");
		Serial.print(heading, 2);
		Serial.println("º");
		Serial.print("Rate: ");
		Serial.print(
				(float) frameCount / (millis() - sampleRateDisplay) * 1000);
		Serial.println(" Hz");
		lastDisplayHeading = millis();
		frameCount = 0;
		sampleRateDisplay = millis();

	}
#endif

	switch (state) {
	case 0: // Initial state, standby
		break;
	case 1: // Autopilot mode
		autopilotState();
		break;
	}

	mainGraphDisplay.draw(mainGraphData);

	// Tick motor driver
	motorDriver->tick();
}

/************************************************************
 *															*
 *			  	Autopilot mode CODE BLOCK					*
 *															*
 ************************************************************/

/*				Compute tiller command 						*/
void computeCommand() {
	headingError = heading - bearing;
	if (abs(headingError) < deadband / 2) {
		headingError = 0;
		command = 0;
	} else {
		if (abs(headingError) > 180) {
			if (bearing > heading)
				headingError += 360;
			else
				headingError -= 360;
		}
		unsigned long now = millis();
		deltaTime = now - previousTime;
		previousTime = now;
		deltaError = headingError - previousError;
		previousError = headingError;

		command = kp * headingError + (kd * deltaError) * 1000 / deltaTime; // by time in sec
		if (command > 100)
			command = 100;
		if (command < -100)
			command = -100;
	}

#if DEBUG == 1
	if (millis() - lastDisplayCommand > DEBUG_DISPLAY_COMMAND_INTERVAL) {
		Serial.print("deltaTime: ");
		Serial.print(deltaTime);
		Serial.print(" heading: ");
		Serial.print(heading);
		Serial.print(" headingError: ");
		Serial.print(headingError);
		Serial.print(" deltaError: ");
		Serial.print(deltaError);
		Serial.print(" command: ");
		Serial.println(command);
		lastDisplayCommand = millis();
	}
#endif

}

/*				Autopilot mode main function				*/
void autopilotState() {

	// Compute command for the actuator

	if (motorDriver->waitingForCommand()) {

		computeCommand();
		motorDriver->correctTo(command);

#if DEBUG == 1
		if (command != 0) {
			Serial.println("Command sent " + String(command));
		}
#endif
	}

}

/*				Current measurement 						*/
float getCorriente(int samplesNumber)
{
   float voltage;
   float corrienteSum = 0;
   for (int i = 0; i < samplesNumber; i++)
   {
      voltage = analogRead(A0) * 5.0 / 1023.0;
      corrienteSum += voltage / SENSIBILITY;
   }
   return(corrienteSum / samplesNumber);
}
