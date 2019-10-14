///////////////////////////////////////////////////////////////////////////
//                   XInput Controller Functions                         //
//                      XInput_Controller.cpp                            //
//      Mathias Voigt -  Technische Universität Dresden - 2019           //
//                   Microsoft XInput API                                //
//https://docs.microsoft.com/en-us/windows/win32/xinput/programming-guide//
///////////////////////////////////////////////////////////////////////////

#include "XInput_Controller.h" //header file, initialize functions

// Link the 'XInput' library
// Note: For Visual Studio 2012 and above, XInput9_1_0 is the library
//       required to make XInput work.
// pragma comment is a linker to this library
#pragma comment(lib, "XInput9_1_0.lib")// part of XInput
                                       // for XInputGetState function
                                       //     XInputSetState function
#pragma comment(lib, "Xinput.lib")     // for XINPUT_BATTERY_INFORMATION

///////////////////////////////////////////////////////////////////////////////////////////////


// Constructor with default ID
XInput_Controller::XInput_Controller()
{

}


// Constructor - set Controller_ID
XInput_Controller::XInput_Controller(int id)
{
	Controller_ID = id - 1;   // XInput IDs 0,1,2,3 user shall use 1,2,3,4

	for (int i = 0; i < button_count; i++)
	{
		prev_buttonStates[i] = false;
		buttonStates[i] = false;
	}
}

// Return gamepad state
XINPUT_STATE XInput_Controller::GetState()
{
    
  //1. XInputGetState function updates the Gamepad State (XINPUT_STATE) 
  //   each time it is called. In Simulink -> every Time Step
  //2. To validate connection, XInputGetState returns a value. 
  //   This value is saved in Variable ConnectionResult
  //3. Initialize Variable of type DWORD
  //   DWORD = 'double word' sized integers (32bit) 
  //Input 1: ID of the Controller from 0 to 3 (max 4 Controller in parallel)
  //      2: Pointer (memory adress) of State that shall be updated
  //Output: (DWORD) ERROR_SUCCESS or (DWORD) ERROR_DEVICE_NOT_CONNECTED 
	XINPUT_STATE new_state;                      //Create State of Type XINPUT_STATE
	ZeroMemory(&new_state, sizeof(XINPUT_STATE));    //Set Memory to 0
	XInputGetState(Controller_ID, &new_state);   //Get new state from XInput

	return new_state;
    
    
}


// Update gamepad state
void XInput_Controller::Update()
{
	// Get current gamepad state
	state = GetState();

	for (int i = 0; i < button_count; i++)
	{
		// Set button state on current frame
        // bit-wise AND operation (binary) via '&'
		buttonStates[i] = (state.Gamepad.wButtons & XINPUT_Buttons[i]) == XINPUT_Buttons[i];
	}
}

// Update gamepad state for next cycle
void XInput_Controller::Refresh()
{
	memcpy(prev_buttonStates, buttonStates, sizeof(prev_buttonStates));
}



// Check if gamepad is connected
bool XInput_Controller::IsConnected()
{
	// Zero memory
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(Controller_ID, &state);

	if (result == ERROR_SUCCESS) // Error_success defined in xinput.h (0L)
		return true;  // Connected
	else
		return false; // Not connected
}


// Return gamepad number
int XInput_Controller::GetConrollerID()
{
	return Controller_ID;
}

int  XInput_Controller::GetBatteryLevel()
{
	XINPUT_BATTERY_INFORMATION BatteryState;            // Get Battery State
	ZeroMemory(&BatteryState, sizeof(XINPUT_BATTERY_INFORMATION));

	XInputGetBatteryInformation(Controller_ID, BATTERY_DEVTYPE_GAMEPAD, &BatteryState);

    // BatteryState.BatteryLevel can only return values 0 to 3
    // zero indicates near empty battery but not dead (controller is connectd)
	int Battery_Level = static_cast<double>(BatteryState.BatteryLevel);

	return Battery_Level;
}

////////////////////////////////// Buttons ////////////////////////////////



// Check if specified button is pressed
bool XInput_Controller::GetButtonPressed(int button)
{   
    // bit-wise AND operation (binary) via '&'
	if (state.Gamepad.wButtons & XINPUT_Buttons[button])
	{
		// Button is pressed
		return true;
	}

	return false; // Button is not pressed
}

// Check if specified button is pressed - CURRENT frame only!
bool XInput_Controller::GetButtonDown(int button)
{
	return !prev_buttonStates[button] && buttonStates[button];
}

////////////////////////////// Analog Axes ////////////////////////////////

double XInput_Controller::Analog_Left_X(int deadzone)
{
	short input = state.Gamepad.sThumbLX;
	if (input > deadzone || input < -deadzone) 
	{
		return (static_cast<double>(input) / 32768.0f);                      // return the input value back
	}
	else
	{
		return 0;                          // if in deadzone, overwrite with 0
	}
}

double XInput_Controller::Analog_Left_Y(int deadzone)
{
	short input = state.Gamepad.sThumbLY;
	if (input > deadzone || input < -deadzone)
	{
		return (static_cast<double>(input) / 32768.0f);
	}
	else
	{
		return 0;                        
	}
}

double XInput_Controller::Analog_Right_X(int deadzone)
{
	short input = state.Gamepad.sThumbRX;
	if (input > deadzone || input < -deadzone)
	{
		return (static_cast<double>(input) / 32768.0f);
	}
	else
	{
		return 0;                          
	}
}

double XInput_Controller::Analog_Right_Y(int deadzone)
{
	short input = state.Gamepad.sThumbRY;
	if (input > deadzone || input < -deadzone)
	{
		return (static_cast<double>(input) / 32768.0f);
	}
	else
	{
		return 0;                          
	}
}

double XInput_Controller::Trigger_Left(int deadzone_trigger)
{
	BYTE input = state.Gamepad.bLeftTrigger;

	if (input > deadzone_trigger)
	{
		return (static_cast<double>(input) / 255.0f);
	}
	else
	{
	    return 0.0f; // Trigger was not pressed
	}
}

double XInput_Controller::Trigger_Right(int deadzone_trigger)
{
	BYTE input = state.Gamepad.bRightTrigger;

	if (input > deadzone_trigger)
	{
		return (static_cast<double>(input) / 255.0f);
	}
	else
	{ 
	    return 0.0f; // Trigger was not pressed
	}
}


//////////////////////////// Set Force Feedback /////////////////////////////

void XInput_Controller::SetForceFeedback(double left, double right)
{

	XINPUT_VIBRATION VibrationState;     	                 // XInput vibration state
	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));   // Zero memory on vibration state
	 
	// Calculate vibration intensity
	int left_motor = int(left * 65535.0f);
	int right_motor = int(right * 65535.0f);

	VibrationState.wLeftMotorSpeed = left_motor;
	VibrationState.wRightMotorSpeed = right_motor;

	// Apply vibration
	XInputSetState(Controller_ID, &VibrationState);
}



