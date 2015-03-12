#pragma once
#include "Sensor.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "OVR_CAPI.h"
#include "Kernel/OVR_Math.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <sstream>
#include <fstream>


class OculusSensor :
	public Sensor
{
public:
	OculusSensor();
	~OculusSensor();
	int OculusSensor::createConnection();
	int OculusSensor::enumerateData();
	int OculusSensor::sendToServer(char* fileLoc);
private:
	ovrHmd hmd;
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
};

