#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "OculusSensor.h"
#include <conio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>

using namespace std;
using namespace rapidjson;


OculusSensor::OculusSensor()
{
}


OculusSensor::~OculusSensor()
{
}

int OculusSensor::getType(){
	return 0;
}

int OculusSensor::createConnection()
{
	/*
	ovr_Initialize();
	hmd = ovrHmd_Create(0);

	wsaData;
	ConnectSocket = INVALID_SOCKET;
	result = NULL;
	ptr = NULL;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("[ERROR] WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}

	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

	//resolve server address and port 
	iResult = getaddrinfo("141.142.21.57", "8000", &hints, &result);

	if (iResult != 0)
	{
		printf("[ERROR] getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("[ERROR] socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}

		send_msg(ConnectSocket, "new");
		id = stol(recv_msg(ConnectSocket));
	}

	if (hmd)
		return 0;
	else
		return -1;
		*/
	return 0;
}

int OculusSensor::enumerateData(const VIVE* myInstance)
{
	/*
	while (hmd){
		Sleep(1000);
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
			PrettyWriter<StringBuffer> writer(s);

			writer.StartObject();
			writer.Key("avatar");
			writer.StartArray();
			writer.StartObject();
			writer.Key("changed");
			writer.Bool(FALSE);
			writer.Key("position");
			writer.StartArray();
			writer.Uint(0);
			writer.Uint(0);
			writer.Uint(0);
			writer.EndArray();
			writer.Key("rotation");
			writer.Uint(0);
			writer.EndObject();
			writer.EndArray();

			writer.Key("create");
			writer.StartArray();
			writer.EndArray();
			
			writer.Key("move");
			writer.StartArray();
			writer.StartObject();
			writer.Key("name");
			writer.String("Object1");
			writer.Key("position");
			writer.StartArray();
			writer.Double(yaw);
			writer.Double(eyePitch);
			writer.Double(eyeRoll);
			writer.EndArray();
			writer.EndObject();
			writer.EndArray();

			writer.Key("Destroy");
			writer.StartArray();
			writer.EndArray();

			writer.EndObject();

			ofstream oculusFile;
			oculusFile.open("C:/Users/amshah4/Documents/GitHub/VIVE/VIVE/Data/OculusData.json");
			oculusFile << s.GetString();
			cout << "[DEBUG] Sending Update to server\n";
			oculusFile.close();
			sendToServer("C:/Users/amshah4/Documents/GitHub/VIVE/VIVE/Data/OculusData.json");
		}
	}
	closeServer();*/
	return 1;
}

void OculusSensor::send_msg(SOCKET sockfd, std::string msg){
	const char* cMsg = msg.c_str();
	size_t length = strlen(cMsg);
	uint32_t nlength = htonl(length);
	send(sockfd, ((char *)&nlength), 4, 0);
	send(sockfd, cMsg, length, 0);
}

std::string OculusSensor::recv_num_bytes(SOCKET sockfd, uint32_t num_bytes){
	char* data = (char*)malloc(num_bytes + 1);
	data[num_bytes] = '\0';

	int num_bytes_recv = 0;
	int curr_num_bytes_recv;
	while (num_bytes_recv < num_bytes){
		curr_num_bytes_recv = recv(sockfd, data + num_bytes_recv, num_bytes - num_bytes_recv, 0);
		num_bytes_recv += curr_num_bytes_recv;
	}
	return std::string(data);
}

std::string OculusSensor::recv_msg(SOCKET sockfd){
	uint32_t length, nlength;

	//getting the first 4 bytes
	int num_bytes_recv = 0;
	int curr_num_bytes_recv;
	while (num_bytes_recv < 4){
		curr_num_bytes_recv = recv(sockfd, ((char*)&nlength) + num_bytes_recv, 4 - num_bytes_recv, 0);
		num_bytes_recv += curr_num_bytes_recv;
	}

	length = ntohl(nlength);
	return recv_num_bytes(sockfd, length);
}

int OculusSensor::closeServer(){
	send_msg(ConnectSocket, "end");
	std::string response = "";
	while (response.compare("goodbye") != 0)
		response = recv_msg(ConnectSocket);

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}



int OculusSensor::sendToServer(char* fileLoc)
{
	send_msg(ConnectSocket, "update");
		
	std::ifstream ifs(fileLoc);
	std::string fileContent((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	send_msg(ConnectSocket, fileContent);
	ifs.close();
	return 0;
}
