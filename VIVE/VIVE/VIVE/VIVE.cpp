#include "VIVE.h"
#include "OculusSensor.h"
#include <iostream>

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


int VIVE::startEnumeration()
{
	for (std::map<char*, Sensor*>::iterator it = sensor_list.begin(); it != sensor_list.end(); ++it){
		it->second->enumerateData();
	}
	return 0;
}
