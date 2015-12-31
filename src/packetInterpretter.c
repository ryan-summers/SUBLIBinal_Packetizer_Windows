#include "packetInterpretter.h"

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
