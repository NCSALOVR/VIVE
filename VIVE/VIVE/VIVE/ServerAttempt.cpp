#include "VIVE.h"
#include "KinectSensor.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <cstdlib>

const char * host = "141.142.21.57";
const char * port = "8000";
VIVE viveInstance(host, port);
KinectSensor sensor;

using namespace std;
using namespace rapidjson;

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

	
	char * filePath = "C:/Users/LOVR/Desktop/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json";

	while (true)
	{
		if (!fexists(filePath)){
			std::string update = viveInstance.getUpdate();
			if (update.length() != 0){
				//update = update.substr(6, update.length() - 1);
				//update = update.substr(0, update.length() - 1);
				//std::cout << "Recieved File" << std::endl;
				const char* jsonData = update.c_str();

				cout << jsonData << endl;

				const long myprofile = viveInstance.id;

				Document documents;

				documents.Parse(jsonData);

				vector<string> createNames;
				vector<double> createRadius;
				vector<tuple<double, double, double>> createPositions;

				vector<string> names;
				vector<double> radius;
				vector<tuple<double, double, double>> positions;
				tuple<double, double, double> avatar = make_tuple(0, 0, 0);
				double rotation = 0;
				bool changed = false;
				bool changeRotation = false;

				for (Value::ConstMemberIterator superitr = documents.MemberBegin();
					superitr != documents.MemberEnd(); ++superitr)
				{
					int currProfile = atoi(superitr->name.GetString());
					string profile = superitr->name.GetString();
					for (Value::ConstMemberIterator itr = documents[profile.c_str()].MemberBegin();
						itr != documents[profile.c_str()].MemberEnd(); ++itr)
					{
						string name = itr->name.GetString();
						if (name.compare("move") == 0)
						{
							const Value& val = itr->value;
							for (SizeType j = 0; j < val.Size(); j++)
							{
								for (Value::ConstMemberIterator moveitr = val[j].MemberBegin();
									moveitr != val[j].MemberEnd(); ++moveitr)
								{
									string name = moveitr->name.GetString();
									//Add To Vector Of Info
									if (name.compare("position") == 0){
										double pos[3];
										for (SizeType k = 0; k < moveitr->value.Size(); k++){
											pos[k] = moveitr->value[k].GetDouble();
										}
										positions.push_back(make_tuple(pos[0], pos[1], pos[2]));
									}
									if (name.compare("name") == 0){
										names.push_back(moveitr->value.GetString());
									}
									if (name.compare("radius") == 0){
										radius.push_back(moveitr->value.GetDouble());
									}
								}
							}
						}
						else if (name.compare("create") == 0){
							const Value& val = itr->value;
							for (SizeType j = 0; j < val.Size(); j++)
							{
								for (Value::ConstMemberIterator moveitr = val[j].MemberBegin();
									moveitr != val[j].MemberEnd(); ++moveitr)
								{
									string name = moveitr->name.GetString();
									//Add To Vector Of Info
									if (name.compare("position") == 0){
										double pos[3];
										for (SizeType k = 0; k < moveitr->value.Size(); k++){
											pos[k] = moveitr->value[k].GetDouble();
										}
										createPositions.push_back(make_tuple(pos[0], pos[1], pos[2]));
									}
									if (name.compare("name") == 0){
										createNames.push_back(moveitr->value.GetString());
									}
									if (name.compare("radius") == 0){
										createRadius.push_back(moveitr->value.GetDouble());
									}
								}
							}
						}
						else if (name.compare("avatar") == 0){
							if (currProfile == myprofile){
								const Value& val = itr->value;
								for (Value::ConstMemberIterator avitr = val.MemberBegin();
									avitr != val.MemberEnd(); ++avitr)
								{
									string name = avitr->name.GetString();
									if (name.compare("position") == 0){
										double av[3];
										const Value & val = avitr->value;
										for (SizeType j = 0; j < val.Size(); j++){
											av[j] = val[j].GetDouble();
										}
										avatar = make_tuple(av[0], av[1], av[2]);
									}
									else if (name.compare("rotation") == 0)
									{
										rotation = avitr->value.GetDouble();
									}
									else if (name.compare("changed") == 0)
									{
										changed = avitr->value.GetBool();
									}
									else if (name.compare("change_rotation") == 0)
									{
										changeRotation = avitr->value.GetBool();
									}
								}
							}
						}
					}
				}

				StringBuffer s;
				PrettyWriter<StringBuffer> writer(s);

				writer.StartObject();

				//avatar
				writer.Key("avatar");
				writer.StartObject();

				writer.Key("position");
				writer.StartArray();
				writer.Double(get<0>(avatar));
				writer.Double(get<1>(avatar));
				writer.Double(get<2>(avatar));
				writer.EndArray();

				writer.Key("rotation");
				writer.Double(rotation);

				writer.Key("change_rotation");
				writer.Double(changeRotation);

				writer.Key("changed");
				writer.Bool(changed);

				writer.EndObject();

				//create
				writer.Key("create");
				writer.StartArray();

				for (int i = 0; i < createNames.size(); i++){
					writer.StartObject();

					//name
					writer.Key("name");
					writer.String(createNames.at(i).c_str());

					writer.Key("radius");
					writer.Double(createRadius.at(i));

					//position
					tuple <double, double, double> tup = createPositions.at(i);

					writer.Key("position");
					writer.StartArray();
					writer.Double(get<0>(tup));
					writer.Double(get<1>(tup));
					writer.Double(get<2>(tup));
					writer.EndArray();

					writer.EndObject();
				}

				writer.EndArray();


				//destroy
				writer.Key("Destroy");
				writer.StartArray();
				writer.EndArray();

				//move
				writer.Key("move");
				writer.StartArray();

				for (int i = 0; i < names.size(); i++){
					writer.StartObject();

					//name
					writer.Key("name");
					writer.String(names.at(i).c_str());

					writer.Key("radius");
					writer.Double(radius.at(i));

					//position
					tuple <double, double, double> tup = positions.at(i);

					writer.Key("position");
					writer.StartArray();
					writer.Double(get<0>(tup));
					writer.Double(get<1>(tup));
					writer.Double(get<2>(tup));
					writer.EndArray();

					writer.EndObject();
				}

				writer.EndArray();
				writer.EndObject();

				string output = s.GetString();

				std::ofstream myfile;
				myfile.open(filePath);
				myfile << output;
				myfile.close();
				//std::cout << update << std::endl;
				//std::cout << "WROTE FILE" << std::endl;
			}
		}
		else{
			continue;
		}
	}
	
}