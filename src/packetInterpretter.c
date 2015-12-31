#include "packetInterpretter.h"

/*
	SUBLIBinal_Packetizer_Windows - GNU GPLv2 Notice
    Copyright (C) 2015  Ryan Summers and James Irwin
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
 
void acquireSynchronization(HANDLE port, char control)
{
	int sync = 0;
	char buf;
	int size;

	//Attempt to read packets from the serial port 
	//We have read the NULL packet. Keep reading packets and echoing
	while (!sync)
	{
		//Read a packet
		size = readPacket(port, control, &buf);
		if (size == 0)
		{
			writePacket(port, control, &buf, size);
		} 
		else if (size == 1 && buf == 1)
		{
			printf("Sending confirmation packet.\n");
			sync = 1;
			writePacket(port, control, &buf, size);
		}
	}
	
}

void getSync (HANDLE portHandle, char controlByte) {
	char buf[100];
	char tempByte, sizeByte;
	int bytesRead;
	int inSync = 0;
	
	printf("Acquiring Stream Sync...");
	while (!inSync) {
		readSerial(portHandle, &tempByte, 1);
		if (tempByte == controlByte) {
			//next should be the size byte
			readSerial(portHandle, &sizeByte, 1);
			//now, we need to read this many bytes
			readSerial(portHandle, buf, sizeByte);
			
			//read one more byte. If this byte is the control byte, we are probably in sync.
			readSerial(portHandle, &tempByte, 1);
			
			if (tempByte == controlByte) {
				inSync = 1;
				
				//read the rest of the packet
				readSerial(portHandle, &sizeByte, 1);
				readSerial(portHandle, buf, sizeByte);
			}
		}
	}
	printf("Sync locked.\n");
}

int readPacket(HANDLE port, char controlByte, char *data) {
	
	char header;
	int sizeByte;
	
	readSerial(port, &header, 1);
	
	//if we encounter the control byte, we are still in sync
	if (header == controlByte) {
		readSerial(port, &sizeByte, 1);
		readSerial(port, data, sizeByte);
		
	} else { //else we need to reacquire stream sync
		getSync(port, controlByte);
		sizeByte = -1;
	}
	
	return sizeByte;
}

int writePacket(HANDLE port, char control, char *data, int byteCount)
{
	char packet[255];
	int status;
	packet[0] = control;
	packet[1] = byteCount;
	memcpy(&(packet[2]), data, byteCount);
	
	status = writeSerial(port, packet, byteCount+2);
	
	return status;
}
