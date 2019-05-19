/*
 * MAINGRAPH.h
 *
 *  Created on: 29 mar. 2019
 *      Author: juantxu
 */

#ifndef GRAPHS_MAINGRAPH_H_
#define GRAPHS_MAINGRAPH_H_

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4

struct POINT {
	int x;
	int y;
	POINT(int p_x, int p_y) :
			x(p_x), y(p_y) {
	}
};

struct MAINGRAPHDATA {
	float *heading;
	short *state;
	float *bearing;
	int *actualCommand;
};

class MAINGRAPH {

public:

	MAINGRAPH() {
		fps = 7;
		lastDisplay = millis();
		yawCompass = 0;
	}

	void setupDisplay();

	void draw(MAINGRAPHDATA data);
protected:
	unsigned int fps;
	unsigned long lastDisplay;

	float yawCompass;

	POINT p1_r = POINT(0, 0);
	POINT p2_r = POINT(0, 0);
	POINT p3_r = POINT(0, 0);
	POINT bearingStart = POINT(0,0);
	POINT bearingEnd = POINT(0,0);
	POINT rotate_point(POINT o, float angle, POINT p);
	Adafruit_SH1106 display = Adafruit_SH1106(OLED_RESET);
};

#endif /* GRAPHS_MAINGRAPH_H_ */
