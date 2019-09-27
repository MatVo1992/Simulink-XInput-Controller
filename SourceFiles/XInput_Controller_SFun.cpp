//////////////////////////////////////////////////////////////////////////
//                       XInput Controller                              //
//                   Simulink S-Function Block                          //
//                  XInput_Controller_SFun.cpp                         //
//      Mathias Voigt -  Technische Universität Dresden - 2019          //
//                                                                      //
//     No Block Input (Controller Feedback) for high performance        //
//////////////////////////////////////////////////////////////////////////


/* Based on Template:
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 * Copyright 1990-2018 The MathWorks, Inc.                             */

/* Interface to XBOX Controller via XInput API
 * https://docs.microsoft.com/en-us/windows/win32/api/_xinput/         */


#define S_FUNCTION_NAME  XInput_Controller_SFun  // define S-Function Name
#define S_FUNCTION_LEVEL 2                        // define S-Function Level


// Includes:
// Need to include simstruc.h for the definition of the SimStruct and
// its associated macro definitions, Simulink functions, data types ...
#include "simstruc.h" 
#include <iostream>              // for std::cout (debugging)  
#include "XInput_Controller.cpp" // function definitions

   
    
    
/*=======================================================================*
 *                          S-function methods                           *
 *=======================================================================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
  // Must be defined before mdlInitializeSizes to compile
  /* Function: mdlCheckParameters =============================================
   * Abstract:
   *    Validate our parameters to verify they are okay.
   */
  static void mdlCheckParameters(SimStruct *S) 
  {
  // In this Method they Input Variables are checked.
  // rule 1: The Controller ID can only have 4 values [1,2,3,4]
  //         (limitation of xinput)
  // rule 2: There must be exactly 1 Parameter
   
  size_t   NrParamters   = mxGetNumberOfElements(ssGetSFcnParam(S, 0));
  real_T * Controller_ID = mxGetPr(ssGetSFcnParam(S, 0)) ; //see mldOutputs
  
  //debug
  //std::cout << "Controller_ID: " << Controller_ID[0] << " mdlCheckParameters" << std::endl; 
  
  // Set NULL Pointer for possible massages
  const char *msg = NULL;
    
    if (NrParamters != 1) 
    {   //Number of Paramaters is not exactly 1
        msg = "This S-function expects only a single parameter.";
    } 
    else
    {
        if (Controller_ID[0] != 1 && Controller_ID[0] != 2 &&
            Controller_ID[0] != 3 && Controller_ID[0] != 4)
        {
            msg = "Possible Controller IDs are 1,2,3,4";
        }
    }
    if (msg != NULL) //If one of the upper if loops is true, ssSetErrorStatus is activated
    {
        ssSetErrorStatus(S, msg);
        return;
    }
  }
#endif /* MDL_CHECK_PARAMETERS */    
    
    
    
/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);            //Number of expected parameters
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) 
    {                                    //compare Block Mask Nr. Parameter and Nr. Parameters defined here
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) //a Error massage is triggered in
        {                                //mdlCheckParameters(S)
            return;
        }
    }
    else // ssGetNumSFcnParams(S) =! ssGetSFcnParamsCount(S)
    {
        return;
         /* If the number of expected input parameters is not equal
         * to the number of parameters entered in the dialog box return.
         * Simulink will generate an error indicating that there is a
         * parameter mismatch.  */
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;      // Nr. Inputs = 1
/*    ssSetInputPortWidth(S, 0, 2);             // Dimension Input[0]= 2  
     //direct input signal access               // -> [1x2] Input
    ssSetInputPortRequiredContiguous(S, 0, true); 
     // Set direct feedthrough flag (1=yes, 0=no).
     // A port has direct feedthrough if the input is used in either
     // the mdlOutputs or mdlGetTimeOfNextVarHit functions. 
    ssSetInputPortDirectFeedThrough(S, 0, 1);
 
 Input Ports set to 0, Input Properties Commented out
 XInputSetState too slow for low sample time /high sample rate
 */

    if (!ssSetNumOutputPorts(S, 3)) return;     // Nr. Outputs = 3
    ssSetOutputPortWidth(S, 0, 6);              // Dimension Output[0] = 6
    ssSetOutputPortWidth(S, 1, 14);             // Dimension Output[1] = 14
    ssSetOutputPortWidth(S, 2, 3);              // Dimension Output[2] = 3
                                                // 1x6 + 1x14 + 1x3 MUX Outputs
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0); 
    ssSetNumPWork(S, 0); // set the Pointer Work Vector to size of 3
                         // we use this vector to share pointers between
                         // callback functions mdlStart & mldOutputs
                         // (the 3 XINPUT_STATES that are used)
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Specify the operating point save/restore compliance to be same as a 
     * built-in block */
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    // get Input Signals via Pointer with Data Type and Name
    // definition of Pointer variable 'real_T *' and cast of 
    // ssGetInputPortRealSignal function output of equal type '(real_T *)'
    
    /* real_T    *feedback = (real_T *)    ssGetInputPortRealSignal (S, 0);   
    
     Input Ports set to 0, Input Properties Commented out
     XInputSetState too slow for low sample time /high sample rate 
     */
    
    // get access to Output Signals via Pointer with Data Type and Name
    real_T    *axes     = (real_T *)    ssGetOutputPortRealSignal(S, 0);
    real_T    *buttons  = (real_T *) ssGetOutputPortRealSignal(S, 1);
    real_T    *info     = (real_T *)    ssGetOutputPortRealSignal(S, 2);
    // Get Simulink Block Parameters  
    real_T  *Controller_ID  = (real_T *) mxGetPr(ssGetSFcnParam(S,0));
      
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
}


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
 real_T  *Controller_ID = (real_T *) mxGetPr(ssGetSFcnParam(S,0));  
 
 // When Block is terminated set Vibration to 0 
 XInput_Controller Controller(Controller_ID[0]);
  
 Controller.SetForceFeedback(0, 0);
  
}


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
