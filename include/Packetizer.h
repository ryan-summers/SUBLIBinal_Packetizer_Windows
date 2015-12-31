//Packetizer Header

#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>


//Forward Declarations
void getLock (HANDLE portHandle, char controlByte);
int getPacket(HANDLE port, char controlByte, char *buffer);

#endif