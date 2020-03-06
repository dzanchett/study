#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

void sendInt(int value, int socket);

int recvInt(int socket);

void sendString(char* value, int size, int socket);

char* recvString(int size, int socket);

void sendVoid(void* value, long size, int socket);

void* recvVoid(long size, int socket);

void sendFile(FILE* pFile, int socket);

void recvFile(FILE* pFile, int socket, int len);

#endif /* SOCKET_UTIL_H */
