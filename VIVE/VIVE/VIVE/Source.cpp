#include "writer.h"
#include "stringbuffer.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace rapidjson;
using namespace std;

int main() {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    
    writer.StartObject();
    writer.String("hello");
    writer.String("world");
    writer.String("t");
    writer.Bool(true);
    writer.String("f");
    writer.Bool(false);
    writer.String("n");
    writer.Null();
    writer.String("i");
    writer.Uint(123);
    writer.String("pi");
    writer.Double(3.1416);
    writer.String("a");
    writer.StartArray();
    for (unsigned i = 0; i < 4; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();

	std::ofstream outputFile;
	outputFile.open("latestOculus.json");
	outputFile << s.GetString();
	outputFile.close();

    return 0;
}