/*
 * L298NMotorDriver.h
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#ifndef MOTORDRIVER_L298NMOTORDRIVER_H_
#define MOTORDRIVER_L298NMOTORDRIVER_H_

#include "MotorDriver.h"

class L298NMotorDriver : public MotorDriver {
public:
	L298NMotorDriver(int pinIn1, int pinIn2);
	~L298NMotorDriver();

	bool correctTo(int command);
	void tick();

protected:
	short pinIn1;
	short pinIn2;
	short pinIn1State;
	short pinIn2State;

};


#endif /* MOTORDRIVER_L298NMOTORDRIVER_H_ */
