#include "VIVE.h"
#include "KinectSensor.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

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

bool fexists(char* filePath)
{
	DWORD dwAttrib = GetFileAttributes(filePath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
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

	
	char * filePath = "C:/Users/amshah4/Desktop/Unreal_stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json";

	while (true)
	{
		if (!fexists(filePath)){
			std::string update = viveInstance.getUpdate();
			std::cout << "Recieved File1" << std::endl;
			if (update.length() != 0){
				update = update.substr(6, update.length() - 1);
				update = update.substr(0, update.length() - 1);
				std::cout << "Recieved File" << std::endl;
				std::ofstream myfile;
				myfile.open(filePath);
				myfile << update;
				myfile.close();
				std::cout << "WROTE FILE" << std::endl;
			}
		}
		else{
			continue;
		}
	}
	
}