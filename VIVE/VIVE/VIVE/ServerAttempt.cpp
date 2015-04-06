#include "VIVE.h"

const char * host = "127.0.0.1";
const char * port = "8000";
VIVE viveInstance(host, port);

void threadCtoSWrapper()
{
	viveInstance.threadCtoS();
}

void threadStoCWrapper()
{
	viveInstance.threadStoC(0.1);
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

	int i = 0;
	while (true)
	{
		std::string update = viveInstance.getUpdate();
		if (update.length() != 0){
			std::cout << "Update: " <<update << std::endl;
		}

		std::cout << "Sending update: " << std::to_string(i) << std::endl;
		viveInstance.sendCommand("update", "{\"id\":" + std::to_string(i) + "}");

		i++;
		Sleep(500);
	}
}