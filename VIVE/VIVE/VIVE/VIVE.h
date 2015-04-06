#pragma once
#include <map>
#include <queue>
#include <tuple>
#include <thread>
#include <mutex> 

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <queue>
#include <tuple>
#include <thread>
#include <mutex> 

#include "socketHelper.h"

#include "Sensor.h"

class VIVE
{
public:
	enum SensorType { OCULUS, KINECT };
	VIVE(const char * myHost, const char * myPort);
	~VIVE();
	int createNewInstance();
	int addSensor(SensorType type, char* description);
	int startEnumeration();
	void oculusEnumeration();
	void kinectEnumeration();
	void threadCtoS(); //Function that reads and sends from update queue
	void threadStoC(float t); //Function that recieves new states from server
	long id;

	std::string getUpdate();
	std::string getDelete();
	std::tuple<std::string, std::string> getCommand();

	void sendCommand(std::string action, std::string data);
private:
	std::map<char*, Sensor*> sensor_list;
	std::queue<std::string> updates;
	std::queue<std::string> deletes;
	std::queue<std::string> fromServerUpdates;
	std::queue<std::string> fromServerDeletes;
	std::queue<std::tuple<std::string, std::string>> commands;
	std::mutex dataLock;
	std::mutex updateLock;

	bool handshake(SOCKET soc);
	void begin(std::string action, float t);

	const char * VIVEhost;
	const char * VIVEport;
};

