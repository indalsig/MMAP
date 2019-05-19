/*
 * RFLogic.cpp
 *
 *  Created on: 12 may. 2019
 *      Author: juantxu
 */

#include "RFLogic.h"


RFLogic::RFLogic(RCSwitch rcSwitch) : rcSwitch(rcSwitch){

}


// explicitly set the number of millisec that have to pass by before a long
// button press is detected.
void RFLogic::setPressTicks(int ticks)
{
  _pressTicks = ticks;
} // setPressTicks


// save function for click event
void RFLogic::attachClick(rfCallbackFunction newFunction)
{
  _clickFunc = newFunction;
} // attachClick

// save function for longPressStart event
void RFLogic::attachLongPressStart(rfCallbackFunction newFunction)
{
  _longPressStartFunc = newFunction;
} // attachLongPressStart

// save function for longPressStop event
void RFLogic::attachLongPressStop(rfCallbackFunction newFunction)
{
  _longPressStopFunc = newFunction;
} // attachLongPressStop

// save function for during longPress event
void RFLogic::attachDuringLongPress(rfCallbackFunction newFunction)
{
  _duringLongPressFunc = newFunction;
} // attachDuringLongPress

// function to get the current long pressed state
bool RFLogic::isLongPressed(){
  return _isLongPressed;
}

int RFLogic::getPressedTicks(){
  return _stopTime - _startTime;
}

void RFLogic::reset(void){
  _state = 0; // restart.
  _startTime = 0;
  _stopTime = 0;
  _isLongPressed = false;
}

void RFLogic::tick() {

	unsigned long now = millis(); // current (relative) time in msecs.
	bool activeLevel = false;
	unsigned long rfValue;

	if (rcSwitch.available()) {
		rfValue = rcSwitch.getReceivedValue();
		activeLevel = true;
		_lastRecievedCodeTime = now;
	}


	// Implementation of the state machine

	if (_state == 0) { // waiting for menu pin being pressed.
		if (activeLevel) {
			_state = 1; // step to state 1
			_startTime = now;
		} // if

	} else if (_state == 1) { // waiting for menu pin being released.

		if (!activeLevel && now - _lastRecievedCodeTime > 200) {
			_state = 2; // step to state 2
			_stopTime = now; // remember stopping time

		} else if ((activeLevel)
				&& ((unsigned long) (now - _startTime) > _pressTicks)) {
			_isLongPressed = true; // Keep track of long press state

			if (_longPressStartFunc)
				_longPressStartFunc(rfValue);
			if (_duringLongPressFunc)
				_duringLongPressFunc(rfValue);
			_state = 6; // step to state 6
			_stopTime = now; // remember stopping time
		} else {
			// wait. Stay in this state.
		} // if

	} else if (_state == 2) {

		if (_clickFunc)
			_clickFunc(rfValue);
		_state = 0; // restart.

	} else if (_state == 6) {
		// waiting for menu pin being release after long press.
		if (!activeLevel && now - _lastRecievedCodeTime > 200) {
			_isLongPressed = false; // Keep track of long press state
			if (_longPressStopFunc)
				_longPressStopFunc(rfValue);
			_state = 0; // restart.
			_stopTime = now; // remember stopping time
		} else {
			// button is being long pressed
			_isLongPressed = true; // Keep track of long press state
			if (_duringLongPressFunc)
				_duringLongPressFunc(rfValue);
		} // if

	} // if
}
