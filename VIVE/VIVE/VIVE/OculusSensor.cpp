#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "OculusSensor.h"
#include <conio.h>
#include <iostream>
#include <Windows.h>

#include "OVR_CAPI.h"
#include "Kernel/OVR_Math.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>

using namespace std;
using namespace rapidjson;
using namespace OVR;


OculusSensor::OculusSensor()
{
}


OculusSensor::~OculusSensor()
{
}

int OculusSensor::createConnection()
{
	ovr_Initialize();
	hmd = ovrHmd_Create(0);

	int rand;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		cout << "Failed";
		cin >> rand;
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		cout << "Could not connect";
		cin >> rand;
		return 0;
	}

	server.sin_addr.s_addr = inet_addr("141.142.21.57");
	server.sin_family = AF_INET;
	server.sin_port = htons(8000);

	if (hmd)
		return 0;
	else
		return -1;
}

int OculusSensor::enumerateData()
{
	if (hmd){
		// Get more details about the HMD.
		ovrSizei resolution = hmd->Resolution;
		ovrBool ovrHmd_ConfigureTracking(ovrHmd hmd, unsigned int supportedTrackingCaps,
			unsigned int requiredTrackingCaps);
		// Start the sensor which provides the Rift?s pose and motion.
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
			ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);
		// Query the HMD for the current tracking state.

		//Position Tracking Camera
		float frustomHorFOV = hmd->CameraFrustumHFovInRadians;
		float frustomVerFOV = hmd->CameraFrustumVFovInRadians;
		float frustomNearZ = hmd->CameraFrustumNearZInMeters;
		float frustomFarZ = hmd->CameraFrustumFarZInMeters;

		float yaw;
		float eyePitch;
		float eyeRoll;

		//Head Position and Sensors
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{

			Posef pose = ts.HeadPose.ThePose;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &eyePitch, &eyeRoll);

			StringBuffer s;
			Writer<StringBuffer> writer(s);

			writer.StartObject();
			writer.String("yaw");
			writer.Double(yaw);
			writer.String("eyePitch");
			writer.Double(eyePitch);
			writer.String("yaw");
			writer.Double(eyeRoll);
			writer.EndObject();

			ofstream oculusFile;
			oculusFile.open("C:/Users/amshah4/Desktop/Unreal stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.txt");
			oculusFile << s.GetString();
			oculusFile.close();
			sendToServer("C:/Users/amshah4/Desktop/Unreal stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.txt");
		}

		return 0;
	}
	else
		return -1;
}


int OculusSensor::sendToServer(char* fileLoc)
{
	int rand;

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cout << "connect error";
	}

	char * msg = "update";

	if (send(s, msg, strlen(msg), 0) < 0)
	{
		cout << "Send failed";
	}

	Sleep(1);

	ifstream f1(fileLoc, std::ios::binary);
	f1.seekg(0, ios::end);
	size_t len = f1.tellg();
	f1.seekg(0, ios::beg);

	stringstream ss;
	ss << len;
	string str = ss.str();
	const char * c_str = str.c_str();

	cout << c_str;

	if (send(s, c_str, strlen(c_str), 0) < 0)
	{
		cout << "Send failed size";
	}

	Sleep(0.001);

	int i = 0;
	while ((i + 1024) < len) {
		char * ret = new char[1024];
		f1.read(ret, 1024);

		if (send(s, ret, 1024, 0) < 0)
		{
			cout << "Failed file send";
		}
		i += 1024;
	}

	if (i < len){
		char * ret = new char[len - i];
		f1.read(ret, len - i);
		cout << ret;
		if (send(s, ret, len - i, 0) < 0)
		{
			cout << "Failed file send";
		}
	}

	f1.close();
	cout << "Complete";
	cin >> rand;
	return 1;
}
