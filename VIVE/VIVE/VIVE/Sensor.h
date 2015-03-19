#pragma once
class Sensor
{
public:
	Sensor();
	~Sensor();
	virtual int createConnection()=0;
	virtual int enumerateData() = 0;
	virtual int getType() = 0;
};

