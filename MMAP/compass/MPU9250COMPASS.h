/*
 * MPU9250Compass.h
 *
 *  Created on: 28 mar. 2019
 *      Author: juantxu
 */

#ifndef MPU9250COMPASS_H_
#define MPU9250COMPASS_H_

#include "COMPASS.h"


class MPU9250COMPASS : public COMPASS {

public:
	MPU9250COMPASS();

	void getHeading(float &heading);

};


#endif /* MPU9250COMPASS_H_ */
