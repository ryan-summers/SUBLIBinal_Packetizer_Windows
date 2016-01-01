
#include "serial.h"

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

int configureSerial(HANDLE port_handle, int baudrate)
{
	DCB config = {0};
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
	DWORD bytesRead = 0, tmp;
	while (bytesRead < byteCount)
	{
		int result = ReadFile(port, &(data[bytesRead]), byteCount-bytesRead, &tmp, 0);
		bytesRead += tmp;
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
