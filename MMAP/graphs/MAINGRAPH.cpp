/*
 * MAINGRAHP.cpp
 *
 *  Created on: 30 mar. 2019
 *      Author: juantxu
 */

#include "MAINGRAPH.h"

POINT MAINGRAPH::rotate_point(POINT o, float angle, POINT p) {
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	p.x -= o.x;
	p.y -= o.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + o.x;
	p.y = ynew + o.y;
	return p;
}

void MAINGRAPH::setupDisplay() {
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
	// init done

	display.clearDisplay();
}

void MAINGRAPH::draw(MAINGRAPHDATA data) {

	if (millis() - lastDisplay < 1000u / fps)
		return;

	lastDisplay = millis();

	MAINGRAPH::display.clearDisplay();
	POINT center = POINT(106, 26);
	int radiusCompass = 18;
	int radiusArrow = 10;
	display.drawCircle(center.x, center.y, radiusCompass, WHITE);

	int linesHeight = 4;
	display.drawFastVLine(center.x, center.y - radiusCompass - linesHeight,
			2 * linesHeight,
			WHITE);
	display.drawFastVLine(center.x, center.y + radiusCompass - linesHeight,
			2 * linesHeight,
			WHITE);

	display.drawFastHLine(center.x - radiusCompass - linesHeight, center.y,
			2 * linesHeight,
			WHITE);
	display.drawFastHLine(center.x + radiusCompass - linesHeight, center.y,
			2 * linesHeight,
			WHITE);

	display.drawChar(center.x - 2, center.y - radiusCompass + linesHeight + 2,
			'N', WHITE, BLACK, 1);

	POINT p1 = POINT(center.x - 1, center.y + 3);
	POINT p2 = POINT(center.x + 1, center.y + 3);
	POINT p3 = POINT(center.x, center.y - radiusArrow);

	yawCompass = *data.heading / 180 * PI;

	float angle = yawCompass;

	//Serial.println(yawCompass * 180 / PI);
	// display.fillTriangle(p1_r.x, p1_r.y, p2_r.x, p2_r.y, p3_r.x, p3_r.y, BLACK);

	p1_r = rotate_point(center, angle, p1);
	p2_r = rotate_point(center, angle, p2);
	p3_r = rotate_point(center, angle, p3);

	display.fillTriangle(p1_r.x, p1_r.y, p2_r.x, p2_r.y, p3_r.x, p3_r.y, WHITE);

	// Bearing line
	if (*data.state == 1) {

		bearingStart = rotate_point(center, *data.bearing / 180 * PI,
				POINT(center.x, center.y - radiusArrow - 4));
		bearingEnd = rotate_point(center, *data.bearing / 180 * PI,
				POINT(center.x, center.y - radiusCompass - 4));

		if (millis() % 2000 > 1000)
			display.drawLine(bearingStart.x, bearingStart.y, bearingEnd.x,
					bearingEnd.y, WHITE);
		else
			display.drawLine(bearingStart.x, bearingStart.y, bearingEnd.x,
					bearingEnd.y, BLACK);
	}

	// Actual command display

	display.drawLine(center.x, 64, center.x, 54, WHITE);
	display.drawLine(center.x - radiusCompass - 3, 64, center.x - radiusCompass - 3, 54, WHITE);
	display.drawLine(center.x + radiusCompass + 3, 64, center.x + radiusCompass + 3, 54, WHITE);

	int width = -(float)(radiusCompass + 3) * (float)(*data.actualCommand) / 100.f;

	if (width > 0)
		display.fillRect(center.x, 56, width, 7, WHITE);
	else if (width == 0)
		display.fillRect(center.x, 56, 0, 7, WHITE);
	else
		display.fillRect(center.x + width, 56 , -width, 7, WHITE);


	display.setTextColor(WHITE);
	display.setTextSize(1);

	display.setCursor(5, 10);
	display.print("HDG: ");
	display.print(*data.heading, 0);

	display.setCursor(5, 19);
	display.print("BRG: ");
	display.print(*data.bearing, 0);

	display.setCursor(5, 28);
	display.print("CMD: ");
	display.print(*data.actualCommand);

	display.setCursor(5, 54);
	display.print("Mode:");

	if (*data.state == 1) {
		display.setTextColor(BLACK, WHITE);
		display.print(" AUTO ");
	} else {
		display.print("MANUAL");
	}
	display.display();
}

