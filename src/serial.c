
#include "../include/Serial.h"
//Serial Functions

HANDLE openSerial(char *port, int baudrate) {
	DCB config = {0};
	HANDLE port_handle = CreateFile(port, 
							 GENERIC_READ|GENERIC_WRITE,
							 0, //do not share the file
							 0, //no security attributes
							 OPEN_EXISTING, //open the file if it exists
							 FILE_ATTRIBUTE_NORMAL, //This is a standard file 
							 0 //No file template
							 );
							 
	if (port_handle == INVALID_HANDLE_VALUE) {
		printf("Encountered invalid handle. Error: %d\n", GetLastError());
		return -1;
	}
	
	config.DCBlength = sizeof(config);
	
	if (GetCommState(port_handle, &config) == 0) {
		printf("Unable to get configuration of port. Error: %d", GetLastError());
		return -1;
	}
	
	config.BaudRate = baudrate;
	config.StopBits = ONESTOPBIT;
	config.Parity = NOPARITY;
	config.ByteSize = 8;
	config.fDtrControl = 0;
	config.fRtsControl = 0;
	
	if (SetCommState(port_handle, &config) == 0) {
		printf("failed to set port configuration\tReason: %d\n", GetLastError());
		return -1;
	}
	
	return port_handle;
}

void readBytes(HANDLE port, int count, char *buffer) {
	int readAmt = 0;
	DWORD bytesRead;
	while (readAmt < count) {
		if (ReadFile(
				port,
				buffer,
				count-readAmt,
				&bytesRead,
				0) == FALSE)
		{
			//printf("Error in Read: %d\n", GetLastError());
		}
		readAmt += bytesRead;
	}
	
}
