/*
 * MainButtonsLogic.h
 *
 *  Created on: 30 mar. 2019
 *      Author: juantxu
 */

#ifndef HID_MAINBUTTONSLOGIC_H_
#define HID_MAINBUTTONSLOGIC_H_

#include <OneButton.h>
#include "Utils.h"


class MainButtonsLogic {

public:

	MainButtonsLogic(short &p_state, float &heading, float &p_bearing);

	short &state;
	float &heading;
	float &bearing;

	void tick();

	OneButton standByButton = OneButton(23, true, true);
	OneButton autoButton = OneButton(22, true, true);
	OneButton starboardButton1 = OneButton(24, true, true);
	OneButton starboardButton10 = OneButton(26, true, true);
	OneButton portButton1 = OneButton(25, true, true);
	OneButton portButton10 = OneButton(27, true, true);

};

// Setup buttons

void setupButtons(MainButtonsLogic* p_mbl);


// Buttons functions

void standByButtonClick();
void autoButtonClick();

void starboardButton1Click();
void starboardButton10Click();
void starboardButton1LongPress();
void starboardButton10LongPress();

void portButton1Click();
void portButton10Click();
void portButton1LongPress();
void portButton10LongPress();




#endif /* HID_MAINBUTTONSLOGIC_H_ */
