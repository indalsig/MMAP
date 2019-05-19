/*
 * RFLogic.h
 *
 *  Created on: 12 may. 2019
 *      Author: juantxu
 */

#ifndef HID_RFLOGIC_H_
#define HID_RFLOGIC_H_

#include <RCSwitch.h>

typedef void (*rfCallbackFunction)(unsigned long);

class RFLogic {

public:

	RFLogic(RCSwitch rcSwitch);


	// ----- Set runtime parameters -----


  // set # millisec after single click is assumed.
  void setClickTicks(int ticks);

  // set # millisec after press is assumed.
  void setPressTicks(int ticks);

  // attach functions that will be called when button was pressed in the
  // specified way.
  void attachClick(rfCallbackFunction newFunction);
  void attachLongPressStart(rfCallbackFunction newFunction);
  void attachLongPressStop(rfCallbackFunction newFunction);
  void attachDuringLongPress(rfCallbackFunction newFunction);

  // ----- State machine functions -----

  /**
   * @brief Call this function every some milliseconds for checking the input
   * level at the initialized digital pin.
   */
  void tick(void);

  /**
   * @brief Call this function every time the input level has changed.
   * Using this function no digital input pin is checked because the current
   * level is given by the parameter.
   */
  void tick(bool level);

  bool isLongPressed();
  int getPressedTicks();
  void reset(void);

private:
  RCSwitch rcSwitch;
  int _pin; // hardware pin number.
  unsigned int _pressTicks = 1000; // number of ticks that have to pass by
								   // before a long button press is detected

  int _buttonPressed = 0;

  bool _isLongPressed = false;

  // These variables will hold functions acting as event source.
  rfCallbackFunction _clickFunc = NULL;
  rfCallbackFunction _longPressStartFunc = NULL;
  rfCallbackFunction _longPressStopFunc = NULL;
  rfCallbackFunction _duringLongPressFunc = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.
  int _state = 0;
  unsigned long _startTime = 0; // will be set in state 1
  unsigned long _stopTime = 0; // will be set in state 2
  unsigned long _lastRecievedCodeTime = 0;
};



#endif /* HID_RFLOGIC_H_ */
