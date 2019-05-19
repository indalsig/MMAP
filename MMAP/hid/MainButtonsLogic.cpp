/*
 * MainButtonsLogic.cpp
 *
 *  Created on: 30 mar. 2019
 *      Author: juantxu
 */

#include "MainButtonsLogic.h"
#include "Globals.h"

// Main buttons logic
MainButtonsLogic* mbl;

MainButtonsLogic::MainButtonsLogic(short &p_state, float &p_heading,
		float &p_bearing) :
		state(p_state), heading(p_heading), bearing(p_bearing) {
}

void MainButtonsLogic::tick() {
	standByButton.tick();
	autoButton.tick();
	starboardButton1.tick();
	starboardButton10.tick();
	portButton1.tick();
	portButton10.tick();

}

void setupButtons(MainButtonsLogic* p_mbl) {
	mbl = p_mbl;

	mbl->autoButton.attachClick(autoButtonClick);

	mbl->starboardButton1.attachClick(starboardButton1Click);
	mbl->starboardButton10.attachClick(starboardButton10Click);
	mbl->starboardButton1.attachDuringLongPress(starboardButton1LongPress);
	mbl->starboardButton10.attachDuringLongPress(starboardButton10LongPress);

	mbl->portButton1.attachClick(portButton1Click);
	mbl->portButton10.attachClick(portButton10Click);
	mbl->portButton1.attachDuringLongPress(portButton1LongPress);
	mbl->portButton10.attachDuringLongPress(portButton10LongPress);
}

void autoButtonClick() {

	buzz();

	switch (mbl->state) {
	case 0:
		mbl->state = 1;
		mbl->bearing = mbl->heading;
		break;
	case 1:
		mbl->state = 0;
		break;
	}
}

void starboardButton1Click() {

	buzz();

	switch (mbl->state) {
	case 0:
		break;
	case 1:
		mbl->bearing = map360(mbl->bearing + 1);
		break;
	}
}

void starboardButton10Click() {

	buzz();

	switch (mbl->state) {
	case 0:
		break;
	case 1:
		mbl->bearing = map360(mbl->bearing + 10);
		break;
	}
}

void portButton1Click() {

	buzz();

	switch (mbl->state) {
	case 0:
		break;
	case 1:
		mbl->bearing = map360(mbl->bearing - 1);
		break;
	}
}

void portButton10Click() {

	buzz();

	switch (mbl->state) {
	case 0:
		break;
	case 1:
		mbl->bearing = map360(mbl->bearing - 10);
		break;
	}
}

unsigned long lastLongPressAction = millis();

void starboardButton1LongPress() {
	if (millis() - lastLongPressAction > 500) {

		buzz();
		starboardButton1Click();
		lastLongPressAction = millis();
	}
}

void starboardButton10LongPress() {
	if (millis() - lastLongPressAction > 500) {

		buzz();
		starboardButton10Click();
		lastLongPressAction = millis();
	}

}

void portButton1LongPress() {
	if (millis() - lastLongPressAction > 500) {

		buzz();
		portButton1Click();
		lastLongPressAction = millis();
	}
}

void portButton10LongPress() {
	if (millis() - lastLongPressAction > 500) {

		buzz();
		portButton10Click();
		lastLongPressAction = millis();
	}
}

