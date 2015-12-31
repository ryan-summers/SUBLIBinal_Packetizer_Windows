//Packetizer Header

#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>


//Forward Declarations

void acquireSynchronization(HANDLE portHandle, char control);


void getSync (HANDLE portHandle, char controlByte);
int readPacket(HANDLE portHandle, char control, char *data);
int writePacket(HANDLE portHandle, char control, char *data, int byteCount);

#endif