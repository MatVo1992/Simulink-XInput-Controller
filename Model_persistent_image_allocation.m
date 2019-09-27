%//////////////////////////////////////////////////////////////////////////
%//                      XInput Controller                               //
%//                   Simulink S-Function Block                          //
%//      Mathias Voigt -  Technische Universität Dresden - 2019          //
%//////////////////////////////////////////////////////////////////////////
%                           based on Tutorial :
%           https://blogs.mathworks.com/simulink/2016/09/08/
%                  including-a-mask-image-in-your-block/
%
%                  Permanent Block Image Allocation
%                 Model_persistent_image_allocation.m
%//////////////////////////////////////////////////////////////////////////

%% This Script will allocate an Image to Simulink .slx file
% only need to run to change / set new image
% Run this script when using Mask Editor

% load the desired picture as data
img_name = 'XBOX_One_Controller_Icon.png';
img_path = fullfile(pwd,'SourceFiles',img_name);
StoredImage = imread(img_path); %2826162 Bytes

% create User Data Structure
UserDataStrut.Image = StoredImage;

% Set the UserData Properties for current model
set_param(gcb, 'UserDataPersistent','on');
set_param(gcb, 'UserData', UserDataStrut);



