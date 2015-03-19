#include "VIVE.h"
#include "OculusSensor.h"
#include <iostream>
#include <thread>

using namespace std;

VIVE::VIVE()
{
}


VIVE::~VIVE()
{
}


int VIVE::createNewInstance()
{
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
		return 0;
	}
	else{
		return -1;
	}
}


void VIVE::kinectEnumeration()
{
	for (std::map<char*, Sensor*>::iterator it = sensor_list.begin(); it != sensor_list.end(); ++it){
		if (it->second->getType() == 1)
			it->second->enumerateData();
	}
}

void VIVE::oculusEnumeration()
{
	for (std::map<char*, Sensor*>::iterator it = sensor_list.begin(); it != sensor_list.end(); ++it){
		cout << it->second->getType();
		if (it->second->getType() == 0)
			it->second->enumerateData();
	}
}

int VIVE::startEnumeration()
{
	return 0;
}
