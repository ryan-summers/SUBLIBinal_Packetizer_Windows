
#include "Serial.h"

int configureSerial(HANDLE port_handle, int baudrate)
{
	DCB config = {0};
	onfig.DCBlength = sizeof(config);
	
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
		printf("failed to set port configuration. Error: %d\n", GetLastError());
		return -1;
	}
	
}

HANDLE openSerial(char *port, int baudrate) {
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
	
	if (configureSerial(port_handle, baudrate) == -1)
	{
		printf("Failed to configure serial port.\n");
		return -1;
	}
	
	return port_handle;
}

int readSerial(HANDLE port, char *data, int byteCount) 
{
	DWORD bytesRead;
	int result = ReadFile(port, data, byteCount, &bytesRead, 0);
	
	if (result == 0)
		printf("Error in Read: %d\n", GetLastError());
	}
	return (int)bytesRead;
}

int writeSerial(HANDLE port, char *data, int byteCount)
{
	DWORD bytesWritten;
	int result = WriteFile(port, data, byteCount, &bytesWritten, 0);
	
	if (result == 0)
		printf("Error in Write: &d\n", GetLastError());
	
	return (int)bytesWritten;
}

void closeSerial(HANDLE portHandle)
{
	CloseHandle(portHandle);
}
