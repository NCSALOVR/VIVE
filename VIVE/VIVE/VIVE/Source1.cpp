#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "VIVE.h"
#include "KinectSensor.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <thread>
#include <dos.h>
#include <conio.h>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output
#include <cstdio>

using namespace std;
using namespace rapidjson;
/*
VIVE viveInstance("1","1");

void occEnumerate(){
	viveInstance.oculusEnumeration();
}

void kinEnumerate(){
	//Library Spawn
	viveInstance.kinectEnumeration();
}

void collectData(){
	viveInstance.createNewInstance();
	//viveInstance.addSensor(VIVE::OCULUS, "Oculus1"); No adding Oculus enumeration
	viveInstance.addSensor(VIVE::KINECT, "Kinect1");
	
	//thread t1(occEnumerate);
	thread t1(kinEnumerate);

	//t1.join();
	t1.join();
}

void updateUnreal(){
	//Update Unreal
}

int server(){
	return 1;
}
*/
/*
int write(){
	StringBuffer s;
	PrettyWriter<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("avatar");
	writer.StartArray();
	writer.StartObject();
	writer.Key("changed");
	writer.Bool(FALSE);
	writer.Key("position");
	writer.StartArray();
	writer.Uint(0);
	writer.Uint(0);
	writer.Uint(0);
	writer.EndArray();
	writer.Key("rotation");
	writer.Uint(0);
	writer.EndObject();
	writer.EndArray();

	writer.Key("create");
	writer.StartArray();
	writer.StartObject();
	writer.Key("name");
	writer.String("head");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("neck");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("shoulderleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("shoulderright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("handleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("handright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("elbowleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("elbowright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("hipleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("hipright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("kneeleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("kneeright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("ankleleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("ankleright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("footleft");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.StartObject();
	writer.Key("name");
	writer.String("footright");
	writer.Key("position");
	writer.StartArray();
	writer.Double(1);
	writer.Double(1);
	writer.Double(1);
	writer.EndArray();
	writer.Key("radius");
	writer.Double(1);
	writer.EndObject();
	writer.EndArray();

	writer.Key("move");
	writer.StartArray();
	writer.EndArray();

	writer.Key("Destroy");
	writer.StartArray();
	writer.EndArray();

	writer.EndObject();

	ofstream oculusFile;
	oculusFile.open("C:/Users/amshah4/Desktop/Unreal_stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json");
	oculusFile << s.GetString();
	oculusFile.close();

	return 0;
}
*/
/*
int main(int argc, char *argv[])
{
	thread t(collectData); //Collects Kinect Data

	while (true){
		Sleep(5000);
		cout << "[Debug] Still Collecting Data... \n";
	}
	
	t.join();
	return 0;
}
*/