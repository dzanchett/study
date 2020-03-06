#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#include "socketUtil.h"

#define MAX_SIZE_TO_SEND 10000

int min(int a, int b){
	if(a<b){
		return a;
	}else{
		return b;
	}
}

void sendInt(int value, int socket){
	send(socket, &value, sizeof(int), 0);
}

int recvInt(int socket){
	int ret;

	recv(socket, &ret, sizeof(int), 0);

	return ret;
}

void sendString(char* value, int size, int socket){
	int i, end;

	for(i=0; i<size; i+=MAX_SIZE_TO_SEND){

		end = min(i+MAX_SIZE_TO_SEND, size);

		send(socket, value+i, end-i, 0);
	}
}

char* recvString(int size, int socket){
	char* ret = malloc(size+10);
	char* buffer = malloc(MAX_SIZE_TO_SEND+10);

	ret[0] = '\0';

	while(strlen(ret) < size){
		memset(buffer, '\0', sizeof buffer);
		int xxx = recv(socket, buffer, min(MAX_SIZE_TO_SEND, size-strlen(ret)), 0);

		buffer[xxx] = '\0';

		strcat(ret, buffer);
	}

	ret[size] = '\0';

	return ret;
}

void sendVoid(void* value, long size, int socket){
	long i, end;

	for(i=0; i<size; i+=MAX_SIZE_TO_SEND){

		end = min(i+MAX_SIZE_TO_SEND, size);

		send(socket, value+i, end-i, 0);
	}
}

void* recvVoid(long size, int socket){
	long sizeReceived;

	sizeReceived = 0;

	void* ret = malloc(size+1);

	while(sizeReceived < size){
		sizeReceived += recv(socket, ret+sizeReceived, min(MAX_SIZE_TO_SEND, size-sizeReceived), 0);
	}

	return ret;
}

void sendFile(FILE* pFile, int socket){
	long lSize, readSize;
	void* buffer;
	size_t result;

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	readSize = 0;

	buffer = (void*) malloc(sizeof(void) * MAX_SIZE_TO_SEND);

	long sizeBuffer;

	sizeBuffer = (long) sizeof(buffer);

	sendInt(sizeof(long), socket);
	sendVoid((void*) &lSize, sizeof(long), socket);
	sendVoid((void*) &lSize, sizeof(long), socket);
	
	while(readSize < lSize){
		result = fread(buffer, sizeof(void), MAX_SIZE_TO_SEND, pFile);

		sendVoid(buffer, result, socket);
	}

	fclose(pFile);
	free(buffer);
}

void recvFile(FILE* pFile, int socket, int len){
	long* lSize;
	long readSize, receivedSize;
	void* buffer;
	size_t result;

	lSize = (long*) recvVoid(len, socket);

	//printf("** %ld\n", *lSize);

	free(lSize);

	lSize = (long*) recvVoid(len, socket);

	//printf("** %ld\n", *lSize);

	readSize = 0;

	buffer = (void*) malloc(sizeof(void) * MAX_SIZE_TO_SEND);
	
	while(readSize < *lSize){
		receivedSize = recv(socket, buffer, MAX_SIZE_TO_SEND, 0);

		readSize += receivedSize;

		//printf("** %ld\n", readSize);

		result = fwrite(buffer, sizeof(char), receivedSize, pFile);
	}

	fclose(pFile);
	free(buffer);
}