#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "VIVE.h"
#include "OculusSensor.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <thread>
#include <dos.h>
#include <conio.h>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>

using namespace std;
using namespace rapidjson;

int MplusM(){
	int rand;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		cout << "Failed";
		cin >> rand;
	}

	SOCKET s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		cout << "Could not connect";
		cin >> rand;
	}

	struct sockaddr_in server;

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8000);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cout << "connect error";
		cin >> rand;

	}

	char * msg = "update";

	if (send(s, msg, strlen(msg), 0) < 0)
	{
		cout << "Send failed";
		cin >> rand;
	}

	ifstream f1("C:/Users/amshah4/Desktop/VIVE/local2.json", std::ios::binary);
	f1.seekg(0, ios::end);
	size_t len = f1.tellg();
	f1.seekg(0, ios::beg);

	stringstream ss;
	ss << len;
	string str = ss.str();
	const char * c_str = str.c_str();

	cout << c_str;
	cin >> rand;

	if (send(s, c_str, strlen(c_str), 0) < 0)
	{
		cout << "Send failed size";
		cin >> rand;
	}


	int i = 0;
	while ((i + 1024) < len) {
		char * ret = new char[1024];
		f1.read(ret, 1024);

		if (send(s, ret, 1024, 0) < 0)
		{
			cout << "Failed file send";
			cin >> rand;
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
			cin >> rand;
		}
	}

	f1.close();
	cout << "Complete";
	cin >> rand;
	return 0;
}

VIVE viveInstance;

void occEnumerate(){
	viveInstance.oculusEnumeration();
}

void kinEnumerate(){
	viveInstance.kinectEnumeration();
}

void collectData(){
	viveInstance.createNewInstance();
	viveInstance.addSensor(VIVE::OCULUS, "Oculus1");
	
	thread t1(occEnumerate);
	thread t2(kinEnumerate);

	t1.join();
	t2.join();
}

void send_msg(SOCKET sockfd, std::string msg){
	const char* cMsg = msg.c_str();
	size_t length = strlen(cMsg);
	uint32_t nlength = htonl(length);
	send(sockfd, ((char *)&nlength), 4, 0);
	send(sockfd, cMsg, length, 0);
}

std::string recv_num_bytes(SOCKET sockfd, uint32_t num_bytes){
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

std::string recv_msg(SOCKET sockfd){
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


void updateUnreal(){
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

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
	iResult = getaddrinfo("127.0.0.1", "8000", &hints, &result);

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

		Sleep(5000);

		send_msg(ConnectSocket, "new");
		std::string idresp = recv_msg(ConnectSocket);

		send_msg(ConnectSocket, "period");
		send_msg(ConnectSocket, "1");
		std:string update = "";
		while (1){
			Sleep(1000);
			update = recv_msg(ConnectSocket);
			if (update.length() > 0){
				StringBuffer s;
				Writer<StringBuffer> writer(s);

				writer.StartObject();
				writer.String(update.c_str());

				ofstream oculusFile;
				oculusFile.open("C:/Users/amshah4/Desktop/Unreal stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json");
				oculusFile << s.GetString();
				oculusFile.close();
			}
		}
	}
}

int server(){
	return 1;
}

int write(){
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
	writer.StartObject();
	writer.Key("name");
	writer.String("Object1");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.EndArray();

	writer.Key("move");
	writer.StartArray();
	writer.EndArray();

	writer.Key("Destroy");
	writer.StartArray();
	writer.EndArray();

	writer.EndObject();

	ofstream oculusFile;
	oculusFile.open("C:/Users/amshah4/Desktop/Unreal_stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json");
	oculusFile << s.GetString();
	oculusFile.close();

	return 0;
}

int main(int argc, char *argv[])
{
	write();

	thread t(collectData);
	thread t1(updateUnreal);

	while (true){
		Sleep(5000);
		cout << "[Debug] Still Collecting Data... \n";
	}
	
	t.join();
	t1.join();
	
	return 0;
}