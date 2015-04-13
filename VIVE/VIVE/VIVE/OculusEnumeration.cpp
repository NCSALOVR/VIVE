#include <conio.h>
#include <iostream>
#include <Windows.h>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>


using namespace rapidjson;
using namespace std;

void Initialization()
{
	/*
	ovr_Initialize();
	ovrHmd hmd = ovrHmd_Create(0);
	if (hmd)
	{
		// Get more details about the HMD.
		ovrSizei resolution = hmd->Resolution;
		ovrBool ovrHmd_ConfigureTracking(ovrHmd hmd, unsigned int supportedTrackingCaps,
			unsigned int requiredTrackingCaps);
		// Start the sensor which provides the Rift?s pose and motion.
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
			ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);
		// Query the HMD for the current tracking state.
		while (1)
		{
			//Position Tracking Camera
			float frustomHorFOV = hmd->CameraFrustumHFovInRadians;
			float frustomVerFOV = hmd->CameraFrustumVFovInRadians;
			float frustomNearZ = hmd->CameraFrustumNearZInMeters;
			float frustomFarZ = hmd->CameraFrustumFarZInMeters;

			//Head Position and Sensors
			ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
			if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
			{

				float yaw;
				float eyePitch;
				float eyeRoll;

				Posef pose = ts.HeadPose.ThePose;
				pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &eyePitch, &eyeRoll);
				cout << yaw;
				cout << eyePitch;
				cout << eyeRoll;
				cout << endl;
				Sleep(10000);

				StringBuffer s;
				Writer<StringBuffer> writer(s);

				writer.StartObject();
				writer.String("hello");
				writer.String("world");
				writer.String("t");
				writer.Bool(true);
				writer.String("f");
				writer.Bool(false);
				writer.String("n");
				writer.Null();
				writer.String("i");
				writer.Uint(123);
				writer.String("pi");
				writer.Double(3.1416);
				writer.String("a");
				writer.StartArray();
				for (unsigned i = 0; i < 4; i++)
					writer.Uint(i);
				writer.EndArray();
				writer.EndObject();

				cout << s.GetString() << endl;
			}

		}
	}
	// Do something with the HMD.

	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
	*/
}