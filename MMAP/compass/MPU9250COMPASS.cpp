/*
 * MPU9250Compass.cpp
 *
 *  Created on: 28 mar. 2019
 *      Author: juantxu
 */

#include "MPU9250COMPASS.h"
#include "RTIMUSettings.h"
#include "RTIMU.h"
#include "RTFusionRTQF.h"
#include "CalLib.h"
#include "RTIMUMPU9250.h"
#include "Utils.h"

RTIMU *imu;                                           // the IMU object
RTFusionRTQF fusion;                                  // the fusion object
RTIMUSettings settings;                               // the settings object



MPU9250COMPASS::MPU9250COMPASS() {
	int errcode;

	settings.m_MPU9250GyroAccelSampleRate = 10;

	settings.m_MPU9250CompassSampleRate = 10;

	imu = RTIMU::createIMU(&settings);                  // create the imu object

	// Setting acceleration calibration data
	RTVector3 accelCalMin = RTVector3(-0.993726, -0.995681, -0.972583);
	RTVector3 accelCalMax = RTVector3(0.984454, 1.003355, 1.036880);
	imu->setAccelCalibration(accelCalMin, accelCalMax);

	Serial.print("ArduinoIMU starting using device ");
	Serial.println(imu->IMUName());
	if ((errcode = imu->IMUInit()) < 0) {
		Serial.print("Failed to init IMU: ");
		Serial.println(errcode);
	}

	if (imu->getCalibrationValid())
		Serial.println("Using compass calibration");
	else
		Serial.println("No valid compass calibration data");

	// Slerp power controls the fusion and can be between 0 and 1
	// 0 means that only gyros are used, 1 means that only accels/compass are used
	// In-between gives the fusion mix.

	fusion.setSlerpPower(0.3);

	// use of sensors in the fusion algorithm can be controlled here
	// change any of these to false to disable that sensor

	fusion.setGyroEnable(true);
	fusion.setAccelEnable(true);
	fusion.setCompassEnable(true);
}

void MPU9250COMPASS::getHeading(float &heading) {

	if (imu->IMURead()) {
		fusion.newIMUData(imu->getGyro(), imu->getAccel(), imu->getCompass(),
				imu->getTimestamp());

		RTVector3 pose = fusion.getFusionPose();



		float currentHeading = map360(pose.z() * 180 / PI);


		heading = currentHeading;
	}
}
