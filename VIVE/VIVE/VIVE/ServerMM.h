#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <thread>
#include "socketHelper.h"

class ServerMM
{
public:
	ServerMM();
	~ServerMM();
private:
	WSADATA wsaData;
};

