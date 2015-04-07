#include "VIVE.h"
#include "OculusSensor.h"
#include "KinectSensor.h"
#include <iostream>
#include <thread>

using namespace std;

VIVE::VIVE(const char * myHost, const char * myPort)
{
	VIVEhost = myHost;
	VIVEport = myPort;
	id = 0;
}


VIVE::~VIVE()
{
}


int VIVE::createNewInstance()
{
	begin("new", 0.015);
	return 0;
}


int VIVE::addSensor(SensorType type, char* description)
{
	if (OCULUS == type){
		sensor_list[description] = new OculusSensor();
		sensor_list[description]->createConnection();
		return 1;
	}
	else if (KINECT == type){
		//sensor_list[description] = new KinectSensor();
		//sensor_list[description]->createConnection();
		return 1;
	}
	else{
		return -1;
	}
}


void VIVE::kinectEnumeration()
{
	for (std::map<char*, Sensor*>::iterator it = sensor_list.begin(); it != sensor_list.end(); ++it){
		if (it->second->getType() == 1)
			it->second->enumerateData(this);
	}
}

void VIVE::oculusEnumeration()
{
	for (std::map<char*, Sensor*>::iterator it = sensor_list.begin(); it != sensor_list.end(); ++it){
		cout << it->second->getType();
		if (it->second->getType() == 0)
			it->second->enumerateData(this);
	}
}

int VIVE::startEnumeration()
{
	return 0;
}

std::string VIVE::getUpdate()
{
	dataLock.lock();
	if (updates.size() == 0){
		dataLock.unlock();
		return "";
	}
	std::string retElem = updates.front();
	updates.pop();
	dataLock.unlock();
	return retElem;
}

std::string VIVE::getDelete()
{
	dataLock.lock();
	if (deletes.size() == 0){
		dataLock.unlock();
		return "";
	}
	std::string retElem = deletes.front();
	deletes.pop();
	dataLock.unlock();
	return retElem;
}

std::tuple<std::string, std::string> VIVE::getCommand()
{
	dataLock.lock();
	if (commands.size() == 0){
		dataLock.unlock();
		return std::tuple<std::string, std::string>("", "");
	}
	std::tuple<std::string, std::string> retElem = commands.front();
	commands.pop();
	dataLock.unlock();
	return retElem;
}

void VIVE::sendCommand(std::string action, std::string data)
{
	dataLock.lock();
	commands.push(std::tuple<std::string, std::string>(action, data));
	dataLock.unlock();
}

bool VIVE::handshake(SOCKET soc)
{
	try{
		sendMsg(soc, std::to_string(id));
		std::string errorCode = recvMsg(soc);
		if (errorCode.compare("ok") != 0){
			std::cout << errorCode << std::endl;
			closesocket(soc);
			std::cout << "Error, please close program" << std::endl;
			return false;
		}
		return true;
	}
	catch (std::exception& e){
		std::cout << "Error in handshake, please close program" << std::endl;
		return false;
	}
}

void VIVE::begin(std::string action, float t)
{
	if (action.compare("end") == 0){
		std::cout << "Goodbye" << std::endl;
		return;
	}

	SOCKET soc = setupClientSocket(VIVEhost, VIVEport);
	if (soc == INVALID_SOCKET){
		return;
	}

	if (action.compare("new") == 0){
		sendMsg(soc, action);
		id = std::stol(recvMsg(soc));
	}
	else{
		cout << "Sending" << endl;
		sendMsg(soc, action);
	}

	std::string errorCode = recvMsg(soc);
	if (errorCode.compare("ok") != 0){
		std::cout << errorCode << std::endl;
		closesocket(soc);
		std::cout << "Please exit the program now" << std::endl;
		return;
	}

	if (action.compare("new") != 0){
		id = std::stol(action);
	}
	std::cout << "You are id " << id << std::endl;
	std::cout << "Use that id to login in the future" << std::endl;

	sendMsg(soc, "end");
	recvMsg(soc);
	closesocket(soc);
}

void VIVE::threadCtoS()
{
	updateLock.lock();
	SOCKET cToSSoc = setupClientSocket(VIVEhost, VIVEport);
	if (cToSSoc == INVALID_SOCKET){
		updateLock.unlock();
		return;
	}
	if (!handshake(cToSSoc)){
		updateLock.unlock();
		return;
	}

	while (true){
		std::tuple<std::string, std::string> command = getCommand();
		if (command == std::tuple<std::string, std::string>("", ""))
			continue;
		std::string action = std::get<0>(command);
		if (action.compare("end") != 0 && action.compare("update") != 0 && action.compare("delete") != 0)
			continue;
		sendMsg(cToSSoc, action);

		if (action.compare("update") == 0 || action.compare("delete") == 0){
			std::string data = std::get<1>(command);
			sendMsg(cToSSoc, data);
		}

		std::string feedback = recvMsg(cToSSoc);
		if (feedback.compare("goodbye") == 0){
			closesocket(cToSSoc);
			updateLock.unlock();
			return;
		}
	}

}

void VIVE::threadStoC(float t)
{
	DWORD msInterval = t * 1000;
	SOCKET sToCSoc = setupClientSocket(VIVEhost, VIVEport);

	setsockopt(sToCSoc, SOL_SOCKET, SO_RCVTIMEO, (char *)&msInterval, sizeof(DWORD));
	if (sToCSoc == INVALID_SOCKET){
		return;
	}
	if (!handshake(sToCSoc)){
		std::cout << "threadStoC finished" << std::endl;
		return;
	}

	sendMsg(sToCSoc, "period");
	sendMsg(sToCSoc, std::to_string(t));

	while (true){
		Sleep(msInterval);
		if (updateLock.try_lock()){
			updateLock.unlock();
			sendMsg(sToCSoc, "end");
			recvMsg(sToCSoc);
			closesocket(sToCSoc);
			break;
		}
		std::string up;
		std::string del;
		try{
			up = recvMsg(sToCSoc);
			del = recvMsg(sToCSoc);
		}
		catch (const std::runtime_error& e) {
			continue;
		}

		dataLock.lock();
		if (up.length() > 0){
			updates.push(up);
		}
		if (del.length() > 0){
			deletes.push(del);
		}
		dataLock.unlock();
	}
	std::cout << "threadStoC finished" << std::endl;
}