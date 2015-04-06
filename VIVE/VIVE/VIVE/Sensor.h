#pragma once
class VIVE;
class Sensor
{
public:
	Sensor();
	~Sensor();
	virtual int createConnection()=0;
	virtual int enumerateData(const VIVE* myInstance)=0;
	virtual int getType() = 0;
};

