/*
 * Utils.cpp
 *
 *  Created on: 31 mar. 2019
 *      Author: juantxu
 */

#include <Arduino.h>
#include "Globals.h"

float map360(float deg) {
  if (deg < 0) deg += 360;
  else if (deg > 360) deg -= 360;
  return deg;
}


void buzz() {
	tone(BUZZER_PIN, 1200, 300);
}

