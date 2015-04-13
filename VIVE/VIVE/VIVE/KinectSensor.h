#pragma once
#include "Sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <sstream>
#include <fstream>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "VIVE.h"


class KinectSensor
{
public:
	KinectSensor();
	~KinectSensor();
	int createConnection();
	int enumerateData(VIVE & myInstance);
	int getType();
	int sendToServer(char* fileLoc);
	void send_msg(SOCKET sockfd, std::string msg);
	std::string recv_num_bytes(SOCKET sockfd, uint32_t num_bytes);
	std::string recv_msg(SOCKET sockfd);
	int closeServer();
private:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	struct addrinfo *result,
		*ptr,
		hints;
	int iResult;
	int id;
};

