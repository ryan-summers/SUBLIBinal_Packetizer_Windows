#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#include "../include/readSerial.h"


HANDLE port;

//Signal Handler for windows exit
void exitHandler(int sig) {
	
	fflush(stdout); //flush the output buffer before continuing
	printf("Exit Signal received. \nCleaning up...");
	
	//close all file handles here
	CloseHandle(port);
	
	printf("Done. \nExitting.\n\n");
	exit(0);
}



void printData(FILE *outputFile, int timeConstant, double timeStamp) {
	fprintf(outputFile, "%lf\t%d", timeStamp, timeConstant);
}

void printChar(char *data, int count) {
	int i =0;
	for (i = 0; i<count; i++) {
		printf("%c", data[i]);
	}
	printf("\n");
	
}

int main(int argc, char *argv[]) {
	int i = 0, baudRate = 9600, packetSize = 0;
	unsigned int result;
	char portName[10];
	char data[50];
	char defaultFileName[] = "waterLevel.dat";
	char *outputFileName = defaultFileName;
	
	signal(SIGINT, &exitHandler); //Register the signal with windows
	
	//get the port from the arguments
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printf("Serial Data Acquisition parameters: -p [PORT] -f [FILE] -b [BAUDRATE]");
			printf("\n[PORT] should be the respective COM port used.");
			printf("\n[FILE] should be the specified output data file. \tDefault: waterLevel.dat");
			printf("\n[BAUDRATE] should be desired baudrate. \tDefault: 9600");
			
		}else if (strcmp(argv[i], "-p") == 0) {
			if (i+1 >= argc) {
				printf("No port entered.");
			} else {
				strcpy(portName, argv[i+1]);
			}
		} else if (strcmp(argv[i], "-b") == 0) {
			if (i+1 >= argc) {
				printf("No baudrate entered.");
				
			} else {
				
				baudRate = atoi(argv[i+1]);
			}
			
		} else if (strcmp(argv[i], "-f") == 0) {
			if (i+1 >= argc) {
				printf("No file input.");
				
			} else {
				outputFileName = argv[i+1];
				
			}
			
		}
				
	}
	
	printf("Parameters: \n");
	printf("\tBaudRate: %d\n", baudRate);
	printf("\tPort: %s\n", portName);
	printf("\tFile: %s\n", outputFileName);
	
	port = openSerial(portName, baudRate);
	if (port == -1) {
		printf("Error opening serial port. Exitting.");
		return 0;
	}
	
	getLock_Arduino(port, '!');
	while (1) {
		//Continually read bytes and print them to the screen
		packetSize = getPacket_Arduino(port, '!', data);
		printChar(data, packetSize);
	
	}	
	
}

