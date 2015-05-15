#include "KinectSensor.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>

using namespace std;
using namespace rapidjson;

/* This file will run in parallel to the Oculus Sensor data collection*/

KinectSensor::KinectSensor()
{
}

KinectSensor::~KinectSensor()
{
}

int KinectSensor::createConnection(){
	return 0;
	/* Should connect to server. Should be done similarly to KinectSensor::createConnection()*/
}

int KinectSensor::enumerateData(VIVE& myInstance){
	/* Must simply send to server the file that is getting writter to on Desktop*/
	while (true){
		std::string filePath = "C:/Users/amshah4/Documents/GitHub/VIVE/update.json";

		std::string data = "";

		std::ifstream ifs(filePath, std::ifstream::in);
		if (ifs.is_open()){
			data.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			ifs.close();
		}
		else{
			//std::cout << "[ERROR] Not a valid file: " << filePath << std::endl;
			continue;
		}

		remove(filePath.c_str());

		if (data.compare("") != 0){
			//cout << data << endl;
			myInstance.sendCommand("update", data);
			//std::cout << "[DEBUG] Sending file to server" << endl;
		}
	}

	return 0;
}

int KinectSensor::getType(){
	return 1; //DO NOT CHANGE
}

int KinectSensor::closeServer(){
	return 0;
}



int KinectSensor::sendToServer(char* fileLoc)
{
	return 0;
}
