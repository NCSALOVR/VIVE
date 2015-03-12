#pragma once
#include <map>
#include "Sensor.h"

class VIVE
{
public:
	enum SensorType { OCULUS, KINECT };
	VIVE();
	~VIVE();
	int createNewInstance();
	int addSensor(SensorType type, char* description);
	int startEnumeration();

private:
	std::map<char*, Sensor*> sensor_list;
};

