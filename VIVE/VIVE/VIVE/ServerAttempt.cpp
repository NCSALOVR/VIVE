#include "VIVE.h"
#include "KinectSensor.h"

const char * host = "127.0.0.1";
const char * port = "8000";
VIVE viveInstance(host, port);
KinectSensor sensor;

void threadCtoSWrapper()
{
	viveInstance.threadCtoS();
}

void threadStoCWrapper()
{
	viveInstance.threadStoC(0.0166);
}

void startKinectEnumeration()
{
	sensor.enumerateData(viveInstance);
}

int main(int argc, char *argv[])
{
	//setting up and initialize winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup() failed");
		exit(1);
	}

	viveInstance.createNewInstance(); 

	std::thread tWrite(threadCtoSWrapper);
	std::thread tRead(threadStoCWrapper);
	
	std::thread tCollect(startKinectEnumeration);

	int i = 0;
	while (true)
	{
		std::string update = viveInstance.getUpdate();
		if (update.length() != 0){
			std::cout << "Update: " <<update << std::endl;
		}
	}
}