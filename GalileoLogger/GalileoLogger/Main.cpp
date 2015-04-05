#pragma region Preprocessor
#define IO_BUFFER_SIZE 1000
#define LOG_FILE_PATH "C:\\SerialLog.txt"
#include "stdafx.h"
#include "arduino.h"
#include <fstream>
#include <ctime>
using namespace std;
#pragma endregion

int _tmain(int argc, _TCHAR* argv[])
{
	return RunArduinoSketch();
}

#pragma region GlobalVars
char bufferIO;
char fileBuffer[IO_BUFFER_SIZE];
int fileBufferCounter = 0;
#pragma endregion

#pragma region Methods
void LogToFile(char data, char* filename)
{
	if (fileBufferCounter < IO_BUFFER_SIZE - 2)
	{
		fileBuffer[fileBufferCounter++] = data;
	}
	else if (fileBufferCounter == IO_BUFFER_SIZE - 2)
	{
		fileBuffer[IO_BUFFER_SIZE - 2] = data;
		ofstream outstream;
		outstream.open(filename, ios::app);
		outstream << fileBuffer;
		Log("%s", fileBuffer);
		outstream.close();
		fileBufferCounter = 0;
	}
}

void InitiateSession(char* filename)
{
	ofstream outstream;
	outstream.open(filename, ios::app);
	time_t timenow = time(0);
	struct tm * now = localtime(&timenow);
	outstream << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << '\t'
		<< now->tm_hour << ':'
		<< now->tm_min << ':'
		<< now->tm_sec
		<< endl;

	Log("File IO timestamp noted");
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
	int temp = Serial.available();
	if (Serial.available() > 0)
	{
		Serial.readBytes(&bufferIO, 1);
		LogToFile(bufferIO, LOG_FILE_PATH);
	}
}

void serialEvent()
{

}