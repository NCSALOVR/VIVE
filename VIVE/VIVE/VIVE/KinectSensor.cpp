#include "KinectSensor.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>

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

/* This file will run in parallel to the Oculus Sensor data collection*/

KinectSensor::KinectSensor()
{
}

KinectSensor::~KinectSensor()
{
}

int KinectSensor::createConnection(){
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
	return 0;
	/* Should connect to server. Should be done similarly to KinectSensor::createConnection()*/
}

int KinectSensor::enumerateData(const VIVE* myInstance){
	/* Must simply send to server the file that is getting writter to on Desktop*/
	
	//sendToServer(FILELOC);
	while (1)
	{
		Sleep(33);
		/*ofstream kinectFile;
		kinectFile.open("C:/Users/amshah4/Desktop/output.json");
		kinectFile << s.GetString();
		cout << "[DEBUG] Sending Update to server\n";
		kinectFile.close();*/
		sendToServer("C:/Users/amshah4/Desktop/output.json");
	}
	
	return 0;
}

int KinectSensor::getType(){
	return 1; //DO NOT CHANGE
}


void KinectSensor::send_msg(SOCKET sockfd, std::string msg){
	const char* cMsg = msg.c_str();
	size_t length = strlen(cMsg);
	uint32_t nlength = htonl(length);
	send(sockfd, ((char *)&nlength), 4, 0);
	send(sockfd, cMsg, length, 0);
}

std::string KinectSensor::recv_num_bytes(SOCKET sockfd, uint32_t num_bytes){
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

std::string KinectSensor::recv_msg(SOCKET sockfd){
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

int KinectSensor::closeServer(){
	send_msg(ConnectSocket, "end");
	std::string response = "";
	while (response.compare("goodbye") != 0)
		response = recv_msg(ConnectSocket);

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}



int KinectSensor::sendToServer(char* fileLoc)
{
	send_msg(ConnectSocket, "update");

	std::ifstream ifs(fileLoc);
	std::string fileContent((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	send_msg(ConnectSocket, fileContent);
	ifs.close();
	return 0;
}
