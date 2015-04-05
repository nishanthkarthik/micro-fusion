#pragma region Preprocessor
#include "stdafx.h"
#include "arduino.h"
#include <fstream>
using namespace std;
#pragma endregion

int _tmain(int argc, _TCHAR* argv[])
{
	return RunArduinoSketch();
}

#pragma region GlobalVars
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
	Serial.begin(CBR_115200, Serial.SERIAL_7O2);
}

// the loop routine runs over and over again forever:
void loop()
{
	if (Serial.available() > 100)
	{
		bufferString = Serial.readStringUntil('\0');
		LogToFile(bufferString, "C:\\SerialLog.txt");
	}
}

void serialEvent()
{

}