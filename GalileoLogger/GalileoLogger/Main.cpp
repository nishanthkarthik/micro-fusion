#pragma region Preprocessor
#include "stdafx.h"
#include "arduino.h"
#include "SoftwareSerial.h"
#include <fstream>
using namespace std;
#pragma endregion

int _tmain(int argc, _TCHAR* argv[])
{
	return RunArduinoSketch();
}

#pragma region GlobalVars
SoftwareSerial auxSerial(10, 11);	//TX,RX
String bufferString;
#pragma endregion

#pragma region Methods
void LogToFile(String data, char* filename)
{
	ofstream outstream;
	outstream.open(filename, ios::app);
	outstream << data;
	Log(data.c_str());
	outstream.close();
}
#pragma endregion

void setup()
{
	auxSerial.begin(115200);
	auxSerial.listen();
}

// the loop routine runs over and over again forever:
void loop()
{
	if (auxSerial.available() > 100)
	{
		bufferString = auxSerial.readStringUntil('\0');
		LogToFile(bufferString, "C:\\SerialLog.txt");
	}
}

void serialEvent1()
{

}