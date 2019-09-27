///////////////////////////////////////////////////////////////////////////
//                   XInput Controller header                            //
//                      XInput_Controller.h                              //
//      Mathias Voigt -  Technische Universität Dresden - 2019           //
//                   Microsoft XInput API                                //
//https://docs.microsoft.com/en-us/windows/win32/xinput/programming-guide//
///////////////////////////////////////////////////////////////////////////



#ifndef XINPUT_CONTROLLER_H
#define XINPUT_CONTROLLER_H
 /* Those are called #include guards.
    Once the header is included, it checks if a unique value (in this case XINPUT_CONTROLLER_H) is defined. 
	Then if it's not defined, it defines it and continues to the rest of the page.*/

#include <Windows.h> // for ZeroMemory Function
#include <Xinput.h>  // for XINPUT_STATE     structure
                     //     XINPUT_VIBRATION structure
                     //     XINPUT_GAMEPAD   structure
                     //     XINPUT_BATTERY_INFORMATION structure


// Array with Key Values
// The bit-Values for every Key are in XInput-h via compiler #define 	
static const WORD XINPUT_Buttons[] = {   // HEX       DEC  Binary
	XINPUT_GAMEPAD_DPAD_UP,              // 0x0001      1  0000000000000001
	XINPUT_GAMEPAD_DPAD_DOWN,            // 0x0002      2  0000000000000010
	XINPUT_GAMEPAD_DPAD_LEFT,            // 0x0004      4  0000000000000100
	XINPUT_GAMEPAD_DPAD_RIGHT,           // 0x0008      8  0000000000001000
	XINPUT_GAMEPAD_START,                // 0x0010     16  0000000000010000
	XINPUT_GAMEPAD_BACK,                 // 0x0020     32  0000000000100000
	XINPUT_GAMEPAD_LEFT_THUMB,           // 0x0040     64  0000000001000000
	XINPUT_GAMEPAD_RIGHT_THUMB,          // 0x0080    128  0000000010000000
	XINPUT_GAMEPAD_LEFT_SHOULDER,        // 0x0100    256  0000000100000000
	XINPUT_GAMEPAD_RIGHT_SHOULDER,       // 0x0200    512  0000001000000000
	XINPUT_GAMEPAD_A,                    // 0x1000   4096  0001000000000000
	XINPUT_GAMEPAD_B,                    // 0x2000   8192  0010000000000000
	XINPUT_GAMEPAD_X,                    // 0x4000  16384  0100000000000000
	XINPUT_GAMEPAD_Y                     // 0x8000  32768  1000000000000000
};
    /* XIput returns a state. All buttons are in superposition in 1 HEX 
       number. To check for one button the HEX state and the HEX code for 
       the key are compared by 'bit-wise AND' (&) 
       If the button is pressed this operation returns 1 for the key
       example:                                      
       GamepadState.Gamepad.wButtons = 0010000000001001  (B, Up and Right)
       B-Button                      = 0010000000000000
       bit-wise AND(ampersand sign &)  -------&--------
                                     = 0010000000000000 --> != 0 (is NOT 0)
                                    -> B is pressed                    */



class XInput_Controller
{
public:
	XInput_Controller();                     // Constructor 
	XInput_Controller(int id);               // Constructor with ID

	////////////////////////////////////// General //////////////////////////////////////////////
	XINPUT_STATE GetState();                 // Return gamepad state

	void Update();                           // Update gamepad state
	void Refresh();                          // Update gamepad state for next cycle

	bool IsConnected();                      // Check if gamepad is connected
	int  GetConrollerID();                   // Return gamepad number
	int  GetBatteryLevel();                  // Get the Battery Level

	////////////////////////////////////// Buttons //////////////////////////////////////////////
	bool GetButtonPressed(int button); 	     // Check if specified button is pressed
	bool GetButtonDown(int button);          // Check if specified button is pressed - CURRENT frame only!

	/////////////////////////////////////// Analog ///////////////////////////////////////////////
	double Analog_Left_X (int deadzone);     // Return X axis of left stick  (if not in deadzone)
	double Analog_Left_Y (int deadzone);     // Return Y axis of left stick  (if not in deadzone)
	double Analog_Right_X(int deadzone);     // Return X axis of right stick (if not in deadzone)
	double Analog_Right_Y(int deadzone);     // Return Y axis of right stick (if not in deadzone)

	double Trigger_Left (int deadzone_trigger);                   // Return value of left trigger
	double Trigger_Right(int deadzone_trigger);                   // Return value of right trigger


	///////////////////////////////////// Set Force Feedback //////////////////////////////////////
    // Set the Controllers Force Feedback (Vibration)
	void SetForceFeedback(double left = 0.0f, double right = 0.0f);


private:

	int Controller_ID;                       // Gamepad number (1,2,3 or 4)

	static const int button_count = sizeof(XINPUT_Buttons)/ sizeof(XINPUT_Buttons[0]);
	                                        //count Nr of Array Elements
	                                        //size of Array (in bit) / size of 1 Array element (in bit)

	bool prev_buttonStates[button_count];   // Button states from previous update/frame
	bool buttonStates[button_count];        // Button states in current update/frame

	XINPUT_STATE state;                     // Current gamepad state

};


#endif

