/*
 * L298NMotorDriver.cpp
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#include <Arduino.h>
#include <L298NMotorDriver.h>
#include "Globals.h"

L298NMotorDriver::L298NMotorDriver(int p_pinIn1, int p_pinIn2) : MotorDriver() {
	pinIn1 = p_pinIn1;
	pinIn2 = p_pinIn2;

	pinMode(pinIn1, OUTPUT);
	pinMode(pinIn2, OUTPUT);

	actualCommand = 0;
	pinIn1State = LOW;
	pinIn2State = LOW;

	digitalWrite(pinIn1, pinIn1State);
	digitalWrite(pinIn2, pinIn2State);

}

L298NMotorDriver::~L298NMotorDriver() {

}

void L298NMotorDriver::tick() {

	if (millis() - lastCommandTime <
			(unsigned long)((float)(maxCommandStep - minCommandStep) * (float)abs(actualCommand)
					/ 100.f + minCommandStep)) {

		if (!commandStart) {
#if DEBUG == 2
			Serial.print("Command started at "); Serial.println(millis());
#endif
			commandStart = true;
		}
		digitalWrite(pinIn1, pinIn1State);
		digitalWrite(pinIn2, pinIn2State);

	} else {
		if (commandStart) {
#if DEBUG == 2
			Serial.print("Command stopped after "); Serial.println(millis() - lastCommandTime);
#endif
			commandStart = false;
		}
		digitalWrite(pinIn1, LOW);
		digitalWrite(pinIn2, LOW);
	}
}

bool L298NMotorDriver::correctTo(int command) {
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
