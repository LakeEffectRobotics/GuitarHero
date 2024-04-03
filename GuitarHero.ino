
/*
 *  Project     Arduino XInput Library
 *  @author     David Madison
 *  @link       github.com/dmadison/ArduinoXInput
 *  @license    MIT - Copyright (c) 2019 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *  Example:      GamepadPins
 *  Description:  Uses all of the available pin inputs to build a 'complete'
 *                Xbox gamepad, with both analog joysticks, both triggers,
 *                and all of the main buttons.
 *
 *                * Joysticks should be your typical 10k dual potentiometers.
 *                  To prevent random values caused by floating inputs,
                    joysticks are disabled by default.
 *                * Triggers can be either analog (pots) or digital (buttons).
 *                  Set the 'TriggerButtons' variable to change between the two.
 *                * Buttons use the internal pull-ups and should be connected
 *                  directly to ground.
 *
 *                These pins are designed around the Leonardo's layout. You
 *                may need to change the pin numbers if you're using a
 *                different board type
 *
 */

#include <XInput.h>
#include <map>

const boolean UseTriggerButtons = true;   // set to false if using analog triggers

const int ADC_Max = 1023;  // 10 bit

// Trigger Pins
const int Pin_TriggerL = A4;
const int Pin_TriggerR = A5;

// Button Pins
const int Pin_ButtonA = 0;
const int Pin_ButtonB = 1;
const int Pin_ButtonX = 2;
const int Pin_ButtonY = 3;

const int Pin_ButtonLB = 4;
const int Pin_ButtonRB = 5;

const int Pin_ButtonStart = 7;


const int buttonCount = 8;
int allButtons[buttonCount];

// debounce setup
boolean buttonState;            // the current reading from the input pin
boolean lastButtonState = HIGH;  // the previous reading from the input pin

std::map<int, boolean> buttonStateHash;
std::map<int, boolean> lastButtonStateHash;
std::map<int, int>  pinToXinputButtonHash;
std::map<int, unsigned long> lastDebounceTimeHash;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;//50;    // the debounce time; increase if the output flickers


void setup() {
	// If using buttons for the triggers, use internal pull-up resistors
	if (UseTriggerButtons == true) {
		pinMode(Pin_TriggerL, INPUT_PULLUP);
		pinMode(Pin_TriggerR, INPUT_PULLUP);
	}
	// If using potentiometers for the triggers, set range
	else {
		XInput.setTriggerRange(0, ADC_Max);
	}

  allButtons[0] = Pin_ButtonA;
  allButtons[1] = Pin_ButtonB;
  allButtons[2] = Pin_ButtonX;
  allButtons[3] = Pin_ButtonLB;
  allButtons[4] = Pin_ButtonRB;
  allButtons[5] = Pin_ButtonStart;
  allButtons[6] = Pin_TriggerL;
  allButtons[7] = Pin_TriggerR;

  

  for(int pinNum = 0; pinNum < buttonCount; pinNum++){
    // Set buttons as inputs, using internal pull-up resistors
    pinMode(allButtons[pinNum], INPUT_PULLUP);

    // init debounce hashes
    buttonStateHash[allButtons[pinNum]] = HIGH;

     // more debounce hashes
    lastButtonStateHash[allButtons[pinNum]] = HIGH;

    lastDebounceTimeHash[allButtons[pinNum]] = 0;
  }
  

  // init this hash so we can look up Xinput button value by pin #
  pinToXinputButtonHash[Pin_ButtonA]=BUTTON_A;
  pinToXinputButtonHash[Pin_ButtonB]=BUTTON_B;
  pinToXinputButtonHash[Pin_ButtonX]=BUTTON_X;
  pinToXinputButtonHash[Pin_ButtonLB]=BUTTON_LB;
  pinToXinputButtonHash[Pin_ButtonRB]=BUTTON_RB;
  pinToXinputButtonHash[Pin_ButtonStart]=BUTTON_START;
  pinToXinputButtonHash[Pin_TriggerL]=TRIGGER_LEFT;
  pinToXinputButtonHash[Pin_TriggerR]=TRIGGER_RIGHT;

	XInput.setAutoSend(false);  // Wait for all controls before sending

	XInput.begin();
}

void loop() {

  for(int pinNum = 0; pinNum < buttonCount; pinNum++){
     // Read pin values and store in variables
    // (Note the "!" to invert the state, because LOW = pressed)
    boolean buttonState = !digitalRead(allButtons[pinNum]);

    debounce(buttonState,allButtons[pinNum]);

    lastButtonStateHash[allButtons[pinNum]] = buttonState;
  }
//	// Read pin values and store in variables
//	// (Note the "!" to invert the state, because LOW = pressed)
//	boolean buttonA = !digitalRead(Pin_ButtonA);
//  boolean buttonB = !digitalRead(Pin_ButtonB);
//  
   // check to see if you just pressed the button
  // (i.e. the input went from HIGH to LOW), and you've waited long enough
  // since the last press to ignore any noise:


 //for (auto it = 
 //debounce(buttonA,Pin_ButtonA);


// 
//  // If the switch changed, due to noise or pressing:
//  if (buttonA != lastButtonStateHash[Pin_ButtonA]) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//     //Serial.print("reset the button state\n");
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer than the debounce
//    // delay, so take it as the actual current state:
//
//    // if the button state has changed:
//    if (buttonA != buttonStateHash[Pin_ButtonA]) {
//      buttonStateHash[Pin_ButtonA] = buttonA;
//      XInput.setButton(BUTTON_A, buttonA);
//    }
//  }
//
////For buttonB now
//  // If the switch changed, due to noise or pressing:
//  if (buttonB != lastButtonStateHash[Pin_ButtonB]) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//     //Serial.print("reset the button state\n");
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer than the debounce
//    // delay, so take it as the actual current state:
//
//    // if the button state has changed:
//    if (buttonB != buttonStateHash[Pin_ButtonB]) {
//      buttonStateHash[Pin_ButtonB] = buttonB;
//      XInput.setButton(BUTTON_B, buttonB);
//    }
//  }


//	boolean buttonX = !digitalRead(Pin_ButtonX);
//	boolean buttonY = !digitalRead(Pin_ButtonY);
//
//	boolean buttonLB = !digitalRead(Pin_ButtonLB);
//	boolean buttonRB = !digitalRead(Pin_ButtonRB);
//
//	boolean buttonStart = !digitalRead(Pin_ButtonStart);
//

//
//
//	// Set XInput buttons
////	XInput.setButton(BUTTON_A, buttonA);
//	XInput.setButton(BUTTON_B, buttonB);
//	XInput.setButton(BUTTON_X, buttonX);
//	XInput.setButton(BUTTON_Y, buttonY);
//
//	XInput.setButton(BUTTON_LB, buttonLB);
//	XInput.setButton(BUTTON_RB, buttonRB);
//
//	XInput.setButton(BUTTON_BACK, buttonBack);
//	XInput.setButton(BUTTON_START, buttonStart);
//
//	XInput.setButton(BUTTON_L3, buttonL3);
//	XInput.setButton(BUTTON_R3, buttonR3);
//
//
//	// Set XInput trigger values
//	if (UseTriggerButtons == true) {
//		// Read trigger buttons
//		boolean triggerLeft  = !digitalRead(Pin_TriggerL);
//		boolean triggerRight = !digitalRead(Pin_TriggerR);
//
//		// Set the triggers as if they were buttons
//		XInput.setButton(TRIGGER_LEFT, triggerLeft);
//		XInput.setButton(TRIGGER_RIGHT, triggerRight);
//	}
//	else {
//		// Read trigger potentiometer values
//		int triggerLeft  = analogRead(Pin_TriggerL);
//		int triggerRight = analogRead(Pin_TriggerR);
//
//		// Set the trigger values as analog
//		XInput.setTrigger(TRIGGER_LEFT, triggerLeft);
//		XInput.setTrigger(TRIGGER_RIGHT, triggerRight);
//	}


	// Send control data to the computer
	XInput.send();

//   // save the reading. Next time through the loop, it'll be the lastButtonState:
//  lastButtonStateHash[Pin_ButtonA] = buttonA;
//  lastButtonStateHash[Pin_ButtonB] = buttonB;
  
}

void debounce(boolean buttonState, int Pin_Button)
{
  
  // If the switch changed, due to noise or pressing:
  if (buttonState != lastButtonStateHash[Pin_Button]) {
    // reset the debouncing timer
    lastDebounceTimeHash[Pin_Button] = millis();
    Serial.print("reset the button state " ); Serial.println(Pin_Button);
  }

  if ((millis() - lastDebounceTimeHash[Pin_Button]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (buttonState != buttonStateHash[Pin_Button]) {
      buttonStateHash[Pin_Button] = buttonState;
      XInput.setButton(pinToXinputButtonHash[Pin_Button], buttonState);
    }
  }
}
