#include "../include/Packetizer_Arduino.h"

void getLock_Arduino (HANDLE portHandle, char controlByte) {
	char buf[100];
	char tempByte, sizeByte;
	int bytesRead;
	int inSync = 0;
	
	printf("Acquiring Stream Sync...");
	while (!inSync) {
		readBytes(portHandle, 1, &tempByte);
		if (tempByte == controlByte) {
			//next should be the size byte
			readBytes(portHandle, 1, &sizeByte);
			sizeByte -= '0';
			//now, we need to read this many bytes
			readBytes(portHandle, sizeByte+2, buf); //read plus two to throw out newline char and CR
			
			//read one more byte. If this byte is the control byte, we are probably in sync.
			readBytes(portHandle, 1, &tempByte);
			
			if (tempByte == controlByte) {
				inSync = 1;
				
				//read the rest of the packet
				readBytes(portHandle, 1, &sizeByte);
				sizeByte -= '0';
				readBytes(portHandle, sizeByte+2, buf);
			}
		}
	}
	printf("Sync locked.\n");
}


int getPacket_Arduino(HANDLE port, char controlByte, char *buffer) {
	
	char header[2];
	
	readBytes(port, 2, header);
	
	//if we encounter the control byte, we are still in sync
	if (header[0] == controlByte) {
		readBytes(port, header[1]-'0'+2, buffer); //get rid of CR and NL chracters
		
	} else { //else we need to reacquire stream sync
		printf("Sync lost.\n");
		getLock_Arduino(port, controlByte);
	}
	
	return header[1]-'0';
}

