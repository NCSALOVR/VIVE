#pragma once
#include "Sensor.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
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
	int OculusSensor::enumerateData(const VIVE* myInstance);
	int OculusSensor::sendToServer(char* fileLoc);
	int OculusSensor::getType();
	void send_msg(SOCKET sockfd, std::string msg);
	std::string recv_num_bytes(SOCKET sockfd, uint32_t num_bytes);
	std::string recv_msg(SOCKET sockfd);
	int closeServer();
private:
	//ovrHmd hmd;
	WSADATA wsaData;
	SOCKET ConnectSocket;
	struct addrinfo *result,
		*ptr,
		hints;
	int iResult;
	int id;
};

