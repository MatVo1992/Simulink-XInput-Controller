# Simulink-XInput-Controller
Simulink Block / S-Function for Windows XInput API (XBOX Controller)

This repo includes 'Level 2 C++ MEX S-Functions' to create Simulink Blocks for Inputs based on 
Microsoft XInput API. This Includes XBOX 360 and XBOX ONE Controllers (Gamepads).

2 S-Functions
- XInput_Controller_SFun (fast execution time)
- XInput_Controller_SFun_Feedback (slow execution, Controller Vibration implemented)

4 Simulink Demo Models
- XInput_Controller_SFun Demo Model
- XInput_Controller_SFun_Feedback Demo Model
- XInput_Controller_SFun Masked Demo Model
    -- includes masked Simulink Block with persitent Image for copy&paste
- XInput_Controller_SFun_Feedback Masked Demo Model
    -- includes masked Simulink Block with persitent Image for copy&paste



Advanteges over 'Joystick Input' (VRJoystick) Block:
- Independent 3rd/4th Axes (meaning left/right Trigger dont cancel each other)
- Access to both XBOX Controller Vibration Motors
- Access to Battery Information (Wireless Only)
- XInput has wide support on Windows Devices (Gampads, Wheels ..)


Tested on Windows 10 1903.

Tested with XBOX ONE Controller via Bluetooth and Xbox Wireless Adapter for Windows 10


XInput by Microsoft:
https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput

XInput Tutorial by Lawrence McCauley:
https://lcmccauley.wordpress.com/2014/01/05/gamepad-input-tutorial/

XBOX ONE Controller Icon by Neolau1119:
https://www.iconfinder.com/Neolau1119

Simulink persistent Image allocation Tutorial by Guy on Simulink:
https://blogs.mathworks.com/simulink/2016/09/08/including-a-mask-image-in-your-block/
