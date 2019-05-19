/*
 * MotorDriver.cpp
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */
#include "MotorDriver.h"
#include "Arduino.h"

MotorDriver::~MotorDriver() {

}

MotorDriver::MotorDriver() {
	lastCommandTime = millis() - maxCommandStep;
	actualCommand = 0;
}

bool MotorDriver::waitingForCommand() {
	if (millis() - lastCommandTime > (unsigned long)maxCommandStep)
		return true;
	else
		return false;
}

