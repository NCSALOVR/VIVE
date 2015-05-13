VIVE
=====

In order to correctly install VIVE and set up everything correctly. Follow these steps.

The VIVE project is split into two parts which are VIVE and Kin4WV2ConsoleSkeleton1. Both are projects that are to be opened with Microsoft Visual Studio. VIVE is written in C++, and currently only written for Windows. Similarly Kin4WV2ConsoleSkeleton1 uses Kinect SDK and hence only works on Windows. The responsibility of Kin4WV2ConsoleSkeleton1 is to gather sensor data and write it out a specific location that is known by VIVE. 

Any additions to sensor data can easily be done by the implementation of any class that implements the abstract class Sensor. Once the correct methods are implemented and the Sensor information is added at launch, VIVE will collect the sensor data using the implementation made above, and send it to the server. However, we currently only support a specific type of JSON, but this needs to be edited/modified in VIVE/ServerAttempt.cpp. 

For the entire workflow. Follow the instructions below to first launch VIVE.

1) Download VIVE and Kin4WV2ConsoleSkeleton1
2) In order to set up VIVE correctly, you need to install RapidJSON, OCULUS SDK, KINECT SDK libraries
3) Link RapidJSON to the VIVE Project
4) In VIVE/ServerAttempt.cpp you need to correctly modify the Server Address and Port
5) In VIVE/ServerAttempt.cpp change filePath to point to the location of                                                           ".../ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json". (For example                          "C:/Users/LOVR/Desktop/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json")
6) Modify VIVE/KinectSensor.cpp so that the filePath points to "../VIVE/update.json"'

Rest of Workflow

7) Launch VIVEView.
8) Start the server.
9) Now launch VIVE and Kin4WV2ConsoleSkeleton1 on all the computers (Order does not matter)

Shutting Down or closing one client is done simply by ending the local VIVE instance. 
