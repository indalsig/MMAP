/*
 * VNH2SP30MotorDriver.h
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#ifndef MOTORDRIVER_VNH2SP30MOTORDRIVER_H_
#define MOTORDRIVER_VNH2SP30MOTORDRIVER_H_

#include "MotorDriver.h"

class VNH2SP30MotorDriver : public MotorDriver {
public:
	VNH2SP30MotorDriver(int pinIn1, int pinIn2, int pinPWM);
	~VNH2SP30MotorDriver();

	bool correctTo(int command);
	void tick();

protected:
	short pinIn1;
	short pinIn2;
	short pinPWM;
	short pinIn1State;
	short pinIn2State;

};


#endif /* MOTORDRIVER_VNH2SP30MOTORDRIVER_H_ */
