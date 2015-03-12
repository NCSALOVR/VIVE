#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS



#include "VIVE.h"
#include "OculusSensor.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <sstream>
#include <fstream>


using namespace std;

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

int main(int argc, char *argv[])
{
	/*VIVE viveInstance;
	viveInstance.createNewInstance();
	viveInstance.addSensor(VIVE::OCULUS, "Oculus1");
	viveInstance.startEnumeration();*/
	
	HRESULT hr = CoInitialize(NULL);
	
	return 0;
}