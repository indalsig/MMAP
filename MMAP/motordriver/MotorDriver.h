/*
 * MotorDriver.h
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#ifndef MOTORDRIVER_MOTORDRIVER_H_
#define MOTORDRIVER_MOTORDRIVER_H_

class MotorDriver {

public:
	MotorDriver();

	virtual ~MotorDriver() = 0;
	virtual bool correctTo(int command) = 0;
	virtual void tick() = 0;

	bool waitingForCommand();

	int actualCommand;
	bool commandStart = false;


protected:
	int maxCommandStep = 200;
	int minCommandStep = 20;
	unsigned long lastCommandTime;
};



#endif /* MOTORDRIVER_MOTORDRIVER_H_ */
