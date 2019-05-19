/*
 * VNH2SP30MotorDriver.cpp
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#include <Arduino.h>
#include <VNH2SP30MotorDriver.h>
#include "Globals.h"



VNH2SP30MotorDriver::VNH2SP30MotorDriver(int p_pinIn1, int p_pinIn2, int p_pinPWM) : MotorDriver() {
	pinIn1 = p_pinIn1;
	pinIn2 = p_pinIn2;
	pinPWM = p_pinPWM;

	pinMode(pinIn1, OUTPUT);
	pinMode(pinIn2, OUTPUT);
	pinMode(pinPWM, OUTPUT);
	pinMode(7, INPUT);

	actualCommand = 0;
	pinIn1State = LOW;
	pinIn2State = LOW;

	digitalWrite(pinIn1, pinIn1State);
	digitalWrite(pinIn2, pinIn2State);
	digitalWrite(pinPWM, LOW);

}

VNH2SP30MotorDriver::~VNH2SP30MotorDriver() {

}

void VNH2SP30MotorDriver::tick() {

	if (millis() - lastCommandTime <
			(unsigned long)((float)(maxCommandStep - minCommandStep) * (float)abs(actualCommand)
					/ 100.f + minCommandStep)) {

		if (!commandStart) {
#if DEBUG == 2
			Serial.print("Command started at "); Serial.println(millis());
#endif
			commandStart = true;
		}
		digitalWrite(pinPWM, HIGH);
		digitalWrite(pinIn1, pinIn1State);
		digitalWrite(pinIn2, pinIn2State);

	} else {
		if (commandStart) {
#if DEBUG == 2
			Serial.print("Command stopped after "); Serial.println(millis() - lastCommandTime);
#endif
			commandStart = false;
		}
		digitalWrite(pinPWM, LOW);
		digitalWrite(pinIn1, LOW);
		digitalWrite(pinIn2, LOW);
	}
}

bool VNH2SP30MotorDriver::correctTo(int command) {
	actualCommand = command;
	lastCommandTime = millis();

	if (command > 0) {
		pinIn1State = HIGH;
		pinIn2State = LOW;
	} else if (command < 0) {
		pinIn1State = LOW;
		pinIn2State = HIGH;
	} else {
		pinIn1State = LOW;
		pinIn2State = LOW;
	}

	return true;

}
