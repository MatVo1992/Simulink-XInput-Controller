
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
#include <iostream>
#include "XInput_Controller.cpp"
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define y_width 1

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output function
 *
 */
void XInput_Controller_2_Outputs_wrapper(real_T *axes,
			real_T *buttons,
			real_T *info,
			const real_T *Controller_ID, const int_T p_width0)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
 // !!! In S-Function Builder No Data Type Definition at this point !!!///

      
    // APIs Controller IDs from 0 to 3
    // substruction of -1 in XInput_Controller.cpp
    XInput_Controller Controller(Controller_ID[0]);
    
    // Update the State
    Controller.Update();
    
/////////////////////////// Gamepad Outputs ///////////////////////////////
   
    int deadzone         = 6144; // define custom deadzone
    int deadzone_trigger = 15;
    // for comparsion: Microsoft defaults
    // Left Stick   Right Stick   Trigger
    // 7849         8689          30
    // Note: 0 <= deadzone <= 65534
    

		axes[0] = Controller.Analog_Left_X(deadzone);
		axes[1] = Controller.Analog_Left_Y(deadzone);
		axes[2] = Controller.Analog_Right_X(deadzone);
		axes[3] = Controller.Analog_Right_Y(deadzone);
		axes[4] = Controller.Trigger_Left (deadzone_trigger);
		axes[5] = Controller.Trigger_Right(deadzone_trigger);
    
        // for loop for all 14 buttons and 
for (int j = 0; j < 14; j++)
		{
		buttons[j] = Controller.GetButtonPressed(j);
		}  
 
        //std::cout << "A:" << buttons[10] << std::endl;
    
////////////////////////// Gamepad Output  /////////////////////////////// 
                  // (Force Feedback / Virbation //  
    
 
    //  Controller.SetForceFeedback(feedback[0], feedback[1]); 
    /*
     Input Ports set to 0, Input Properties Commented out
     XInputSetState too slow for low sample time /high sample rate 
    */
        
        
//////////////////////// Gamepad Info ///////////////////////////////////
  
		info[0] = Controller.IsConnected();
        
        // 4 possible Battery Levels, but batterylevel = 0 is not empty,
        // controller is still connected
        // 0 ->  25%
        // 1 ->  50%
        // 2 ->  75%
        // 3 -> 100%
        // not connected -> 0
        if (info[0] == 1)
            {
            info[1] = (Controller.GetBatteryLevel()+1)*25;
            }
            else
            info[1] = 0;
        
		info[2] = Controller.GetConrollerID();
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
 * Terminate function
 *
 */
void XInput_Controller_2_Terminate_wrapper(const real_T *Controller_ID, const int_T p_width0)
{
/* %%%-SFUNWIZ_wrapper_Terminate_Changes_BEGIN --- EDIT HERE TO _END */
 // !!! In S-Function Builder No Data Type Definition at this point !!!///

 // When Block is terminated set Vibration to 0 
 XInput_Controller Controller(Controller_ID[0]);
  
 Controller.SetForceFeedback(0, 0);
/* %%%-SFUNWIZ_wrapper_Terminate_Changes_END --- EDIT HERE TO _BEGIN */
}

