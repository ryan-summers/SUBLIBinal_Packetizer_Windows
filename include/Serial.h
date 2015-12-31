//Header file for Serial.c

#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>


//Forward Declaractions
int configureSerial(HANDLE port, int baudRate);
int readSerial(HANDLE port, char *data, int byteCount);
int writeSerial(HANDLE port, char *data, int byteCount);
HANDLE openSerial(char *port, int baudrate);
void closeSerial(HANDLE port);

#endif