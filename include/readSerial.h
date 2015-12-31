//Header for all of the used functions within Read_Serial_Windows

#ifndef READ_SER_WIN
#define READ_SER_WIN

#include "Serial.h"
#include "Packetizer_Arduino.h"

//Forward Declarations
void exitHandler(int signal);
void printData(FILE *outputFile, int timeConstant, double timeStamp);

#endif