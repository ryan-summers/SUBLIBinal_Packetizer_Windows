//Header file for Serial.c

#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>


//Forward Declaractions
void readBytes(HANDLE port, int count, char *buffer);
HANDLE openSerial(char *port, int baudrate);

#endif