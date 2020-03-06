#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>

#include "socketUtil.h"
#include "dataStructures.h"

#define TRUE 1
#define FALSE 0
#define SIZE_MAX_STRING_COMAND 100

#define OPTION_LIST 1
#define OPTION_FIND 2
#define OPTION_SEND 3
#define OPTION_ERASE 4
#define OPTION_EXIT 5
#define OPTION_SEND2 6

struct File{
	char* nameOfFile;
	int onTheServer;
};

struct ThreadArgument_t1{
    int mysocketServer;
    struct sockaddr_in destServer;
    socklen_t socksizeServer;
};

struct ThreadArgument_t2{
    char* nameOfFileToSend;
    int mysocketServer;
    struct sockaddr_in* destServer;
    socklen_t socksizeServer;
};

struct ThreadArgument_t3{
    char* portArgument;
    char* ipOriginOfFile;
    char* nameOfFileFind;
};

struct Node{
    void* value;
    struct Node * nextNode;
};

typedef struct File File;
typedef struct ThreadArgument_t1 ThreadArgument1;
typedef struct ThreadArgument_t2 ThreadArgument2;
typedef struct ThreadArgument_t3 ThreadArgument3;
typedef struct Node Node;

char* directoryOfFiles;
char* path;
Node files;
Node irets;
Node threads;

pthread_mutex_t lock;

void printAtributes(char *t){
	t = strtok(NULL, " \n");

	while(t != NULL){
		printf("\t%s\n", t);
		t = strtok(NULL, " \n");
	}

	return;
}

File* findFile(char* name){
	Node* att;
	File* attValue;

	att = files.nextNode;

	while(att!=NULL){
		attValue = att->value;

		if(strcmp(attValue->nameOfFile, name) == 0){
			return attValue;
		}

		att = att->nextNode;
	}

	return NULL;
}

void* threadFind( void *ptr ){
	char* portArgument;
	char* ipOriginOfFile;
	char* nameOfFileFind;

	ThreadArgument3* arg = (ThreadArgument3*) ptr;

	portArgument = arg->portArgument;
	ipOriginOfFile = arg->ipOriginOfFile;
	nameOfFileFind = arg->nameOfFileFind;

	struct sockaddr_in destFile;

	int fileSocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&destFile, 0, sizeof(destFile));                /* zero the struct */
	destFile.sin_family = AF_INET;
	destFile.sin_addr.s_addr = inet_addr(ipOriginOfFile); //htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
	destFile.sin_port = htons( atoi( portArgument ) );                /* set destination port number */

	int connectResultFile = connect(fileSocket, (struct sockaddr *)&destFile, sizeof(struct sockaddr_in));

	if( connectResultFile == - 1 ){
		printf("CLIENT ERROR: %s\n", strerror(errno));

		return (void*) 0;
	}

	sendInt(1, fileSocket);

	int portToReceive = recvInt(fileSocket);

	//printf("** %d\n", portToReceive);

	close(fileSocket);

	sleep(0.1);

	fileSocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&destFile, 0, sizeof(destFile));                /* zero the struct */
	destFile.sin_family = AF_INET;
	destFile.sin_addr.s_addr = inet_addr(ipOriginOfFile); //htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
	destFile.sin_port = htons( portToReceive );                /* set destination port number */

	connectResultFile = connect(fileSocket, (struct sockaddr *)&destFile, sizeof(struct sockaddr_in));

	int tttttt = 0;

	double waitTime = 0.1;

	while( connectResultFile == - 1 ){
		sleep(waitTime);

		waitTime += 0.1;

		connectResultFile = connect(fileSocket, (struct sockaddr *)&destFile, sizeof(struct sockaddr_in));

		tttttt++;

		if(tttttt == 30){
			printf("CLIENT ERROR: %s\n", strerror(errno));
			
			return (void*) 0;
		}
	}

	int len = strlen(nameOfFileFind);
	sendInt(len, fileSocket);
	sendString(nameOfFileFind, len, fileSocket);

	len = recvInt(fileSocket);

	if(len == -1){
		printf("An error occurred on the origin client\n");
		close(fileSocket);
		return (void*) 0;
	}

	char* fileLocation;

	fileLocation = (char*) malloc((strlen(directoryOfFiles) + strlen(nameOfFileFind) + 2) * sizeof(char));

	strcpy(fileLocation, directoryOfFiles);
	strcat(fileLocation, "/");
	strcat(fileLocation, nameOfFileFind);

	FILE* pfile;

	pfile = fopen(fileLocation, "wb");

	recvFile(pfile, fileSocket, len);

	close(fileSocket);

	printf("File %s received succesfully\n", nameOfFileFind);
}

void* threadSocket( void *ptr ){
	int len, i, mysocketServer;
	char* nameOfFileToSend;
	struct sockaddr_in* destServer;
	socklen_t socksizeServer;
	Node* at;
	Node* ant;
	File* atValue;

	ThreadArgument2* arg = (ThreadArgument2*) ptr;

	nameOfFileToSend = arg->nameOfFileToSend;
	mysocketServer = arg->mysocketServer;
	destServer = arg->destServer;
	socksizeServer = arg->socksizeServer;

	listen(mysocketServer, 1);

	//printf("Server is waiting for conections!\n");

	int consocket = accept(mysocketServer, (struct sockaddr *)destServer, &socksizeServer);

	len = recvInt(consocket);
	nameOfFileToSend = recvString(len, consocket);

	pthread_mutex_lock(&lock);

	at = files.nextNode;

	while(at != NULL){
		atValue = at->value;

		if(strcmp(atValue->nameOfFile, nameOfFileToSend) == 0){
			break;
		}

		at = at->nextNode;
	}

	pthread_mutex_unlock(&lock);

	FILE* pFile;
	long lSize;
	void* buffer;
	size_t result;

	char* fileLocation;

	fileLocation = (char*) malloc((strlen(directoryOfFiles) + strlen(nameOfFileToSend) + 2) * sizeof(char));

	strcpy(fileLocation, directoryOfFiles);
	strcat(fileLocation, "/");
	strcat(fileLocation, nameOfFileToSend);

	pFile = fopen(fileLocation, "rb");

	if(pFile == NULL){
		printf("pFile == NULL (%s)\n", fileLocation);
		sendInt(-1, consocket);
		close(consocket);
		return (void*) 0;
	}

	sendFile(pFile, consocket);

	close(consocket);

	printf("Conection Closed");

	return (void*) 0;
}

void* threadServerFunction( void *ptr ){
	int mysocketServer, len, i;
	char * nameOfFileToSend;
	struct sockaddr_in destServer;
	socklen_t socksizeServer;
	Node* at;
	Node* ant;
	File* atValue;

	ThreadArgument1* arg = (ThreadArgument1*) ptr;

	mysocketServer = arg->mysocketServer;
	destServer = arg->destServer;
	socksizeServer = arg->socksizeServer;

	/* start listening, allowing a queue of up to 1 pending connection */
	listen(mysocketServer, 1);

	//printf("%s\n", generateString(TAM_STRING));

	int consocket = accept(mysocketServer, (struct sockaddr *)&destServer, &socksizeServer);

	while(consocket){
		int option, i;

		option = recvInt(consocket);

		if(option == 1){
			int aleatoryPort = (rand()%65412) + 1024;
			int socketServerNumber;

			sendInt(aleatoryPort, consocket);

			struct sockaddr_in* destAleatoryServer;

			destAleatoryServer = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

			memset(destAleatoryServer, 0, sizeof(destAleatoryServer));           /* zero the struct before filling the fields */
			destAleatoryServer->sin_family = AF_INET;                /* set the type of connection to TCP/IP */
			destAleatoryServer->sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
			destAleatoryServer->sin_port = htons(aleatoryPort);           /* set the server port number */

			socketServerNumber = socket(AF_INET, SOCK_STREAM, 0);

			/* bind serv information to mysocket */
			bind(socketServerNumber, (struct sockaddr *)destAleatoryServer, sizeof(struct sockaddr));

		    ThreadArgument2 arg2;

		    arg2.nameOfFileToSend = nameOfFileToSend;
		    arg2.mysocketServer = socketServerNumber;
		    arg2.destServer = destAleatoryServer;
		    arg2.socksizeServer = socksizeServer;

		    int* tempInt;
		    pthread_t* tempPthread;

		    tempInt = (int*) malloc(sizeof(int));
		    tempPthread = (pthread_t*) malloc(sizeof(pthread_t));

		    *tempInt = pthread_create(tempPthread, NULL, threadSocket, (void*) &arg2);

		    if(*tempInt){
		        fprintf(stderr,"Error - pthread_create() return code: %d\n", *tempInt);
		        exit( EXIT_FAILURE );
		    }

		    addToList(&irets, (void*) tempInt);
		    addToList(&threads, (void*) tempPthread);
		}else if(option == 2){
			int len = recvInt(consocket);
			char* nameOfFileErased = recvString(len, consocket);

			pthread_mutex_lock(&lock);

			at = files.nextNode;
			ant = &files;

			while(at!=NULL){
				atValue = at->value;

				if(strcmp(atValue->nameOfFile, nameOfFileErased) == 0){
					atValue->onTheServer = 0;

					char* fileToRemove;

					fileToRemove = (char*) malloc(sizeof(char) * (strlen(nameOfFileErased) + strlen(path) + 10));

					strcpy(fileToRemove, path);
					strcat(fileToRemove, nameOfFileErased);

					remove(fileToRemove);

					ant->nextNode = at->nextNode;
					free(at);
				}

				ant = at;
				at = at->nextNode;
			}

			pthread_mutex_unlock(&lock);
		}

		close(consocket);

	    consocket = accept(mysocketServer, (struct sockaddr *)&destServer, &socksizeServer);
	}
}

int main(int argc, char *argv[]){
	int mysocket, temp, flag, i, j;
	char comando[SIZE_MAX_STRING_COMAND + 1], *token;
	void* tempVoid;
	struct sockaddr_in dest;
	Node* at;
	Node* ant;
	File* atValue;

	pthread_mutex_init(&lock, NULL);

	if( argc != 3 ){ 
		printf("USAGE: server directory_to_share port_number\n");

		return EXIT_FAILURE;
    }

    directoryOfFiles = (char*) malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(directoryOfFiles, argv[1]);

    path = (char*) malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(path, argv[1]);
    strcat(path, "/");

	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));                /* zero the struct */
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("192.168.0.10"); //htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
	dest.sin_port = htons( atoi( argv[ 2 ] ) );                /* set destination port number */

	int connectResult = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

	if( connectResult == - 1 ){
		printf("CLIENT ERROR: %s\n", strerror(errno));

		return EXIT_FAILURE;
	}

	char* addressDirectory = argv[1];
	DIR* d;
	struct dirent *dir;

	d = opendir(addressDirectory);

	if(d == NULL){
		printf("DIRECTORY NOT FOUND!\n");

		return EXIT_FAILURE;
	}

	if(d){
		while((dir = readdir(d)) != NULL){
			if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
				continue;
			}

			atValue = (File*) malloc(sizeof(File));

			atValue->nameOfFile = (char*) malloc(strlen(dir->d_name) * sizeof(char));
			strcpy(atValue->nameOfFile, dir->d_name);
			atValue->onTheServer = 0;

			sendInt(OPTION_SEND, mysocket);

			int len = strlen(atValue->nameOfFile);

			sendInt(len, mysocket);

			sendString(atValue->nameOfFile, len, mysocket);

			atValue->onTheServer = 1;

			addToList(&files, (void*) atValue);
		}
		closedir(d);
   }

   srand( time( NULL ));

	struct sockaddr_in destServer; /* socket info about the machine connecting to us */
	struct sockaddr_in servServer; /* socket info about our server */
	int mysocketServer;            /* socket used to listen for incoming connections */
	socklen_t socksizeServer = sizeof(struct sockaddr_in);

	memset(&servServer, 0, sizeof(servServer));           /* zero the struct before filling the fields */
	servServer.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
	servServer.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
	servServer.sin_port = htons( atoi( argv[2] ));           /* set the server port number */

	mysocketServer = socket(AF_INET, SOCK_STREAM, 0);

	/* bind serv information to mysocket */
	bind(mysocketServer, (struct sockaddr *)&servServer, sizeof(struct sockaddr));

	pthread_t threadServer;

	ThreadArgument1 arg1;

	arg1.mysocketServer = mysocketServer;
	arg1.destServer = destServer;
	arg1.socksizeServer = socksizeServer;

    int iretServer = pthread_create(&threadServer, NULL, threadServerFunction, (void*) &arg1);

    if(iretServer){
        fprintf(stderr,"Error - pthread_create() return code: %d\n", iretServer);
        exit( EXIT_FAILURE );
    }

	printf("\e[1;1H\e[2J");

	printf("Shell - Operating Systems\n");
	printf("File Manager - v1.0\n");
	printf("Developer: Diego Zanchett\n");
	printf("\n");

	while(TRUE){
		
		while(TRUE){
			printf(">>> ");

			fgets(comando, SIZE_MAX_STRING_COMAND, stdin);
			//adicionaLista(&historico, comando);

			token = strtok(comando, " \n");

			if(token != NULL){
				break;
			}
		}

		flag = 0;

		if(strcmp(token, "list")==0){
			if(strtok(NULL, " \n") != NULL){
				printf("Invalid Command\n");
				flag = 1;
				continue;
			}

			if(flag == 1){
				continue;
			}

			sendInt(OPTION_LIST, mysocket);

			int len;

			len = recvInt(mysocket);
			char* myIp = recvString(len, mysocket);

			int nFilesOnServer = recvInt(mysocket);

			flag = 0;

			for(i=0; i<nFilesOnServer; i++){
				int idFileServer;
				char* nameFileServer;
				char* ipOriginFileServer;

				idFileServer = recvInt(mysocket);

				len = recvInt(mysocket);
				nameFileServer = recvString(len, mysocket);

				len = recvInt(mysocket);
				ipOriginFileServer = recvString(len, mysocket);

				int fileSent = recvInt(mysocket);

				if(strcmp(ipOriginFileServer, myIp) == 0){
					continue;
				}

				if(flag == 0){
					printf("Files avilables on other clients folders:\n\n");
				}

				flag++;

				printf("\tId of File: %d\n", idFileServer);
				printf("\tName of File: %s\n", nameFileServer);
				printf("\tIP of Origin: %s\n", ipOriginFileServer);
				if(fileSent == 1) printf("\tFile Received!\n");
				printf("\n");
			}

			if(flag==0){
				printf("There aren't availables files on the server.\n");
			}

			printf("\n");

			d = opendir(addressDirectory);

			if(d == NULL){
				printf("DIRECTORY NOT FOUND!\n");

				return EXIT_FAILURE;
			}

			if(d){
				while((dir = readdir(d)) != NULL){
					if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
						continue;
					}

					File* idx = findFile(dir->d_name);

					if(idx != NULL){
						continue;
					}

					atValue = (File*) malloc(sizeof(File));

					atValue->nameOfFile = (char*) malloc(strlen(dir->d_name) * sizeof(char));
					strcpy(atValue->nameOfFile, dir->d_name);
					atValue->onTheServer = 0;
					

					sendInt(OPTION_SEND, mysocket);

					len = strlen(atValue->nameOfFile);

					sendInt(len, mysocket);

					sendString(atValue->nameOfFile, len, mysocket);

					atValue->onTheServer = 1;

					addToList(&files, (void*) atValue);
				}
				closedir(d);
		   }

			if(getSizeOfList(&files)>0){
				printf("Files on this client folder:\n\n");

				at = files.nextNode;

				while(at!=NULL){
					atValue = at->value;

					printf("\tName of File: %s\n", atValue->nameOfFile);
					printf("\tStatus: %s\n", atValue->onTheServer == 1 ? "Shared" : "Not Shared");
					printf("\n");

					at = at->nextNode;
				}
			}else{
				printf("There aren't availables files on this client folder\n");
			}

			printf("\n");
		}else if(strcmp(token, "find")==0){
			int len, idOfFile, status;
			char* fileToFind;
			char* ipOriginOfFile;
			char* nameOfFileFind;

			flag = 0;

			fileToFind = strtok(NULL, " \n");

			if(strtok(NULL, " \n") != NULL || fileToFind == NULL){
				printf("Invalid Command\n");
				flag = 1;
				continue;
			}

			if(flag == 1){
				continue;
			}

			idOfFile = 0;

			len = strlen(fileToFind);

			len--;

			for(i=len; i>=0; i--){
				idOfFile += (fileToFind[i] - '0') * pow(10, len - i);
			}

			sendInt(OPTION_FIND, mysocket);

			sendInt(idOfFile, mysocket);

			status = recvInt(mysocket);

			if(status == -1){
				printf("File not found on the server\n");
				continue;
			}

			// paralelizar	

			len = recvInt(mysocket);
			ipOriginOfFile = recvString(len, mysocket);

			len = recvInt(mysocket);
			nameOfFileFind = recvString(len, mysocket);

			printf("File found on client %s\n", ipOriginOfFile);

			ThreadArgument3* arg3;

			arg3 = (ThreadArgument3*) malloc(sizeof(arg3));

			arg3->portArgument = (char*) malloc(sizeof(char) * strlen(argv[2]));

			strcpy(arg3->portArgument, argv[2]);

			arg3->ipOriginOfFile = ipOriginOfFile;
			arg3->nameOfFileFind =  nameOfFileFind;

			int* tempInt;
		    pthread_t* tempPthread;

		    tempInt = (int*) malloc(sizeof(int));
		    tempPthread = (pthread_t*) malloc(sizeof(pthread_t));

		    *tempInt = pthread_create(tempPthread, NULL, threadFind, (void*) arg3);
		}else if(strcmp(token, "send")==0){
			int len;
			char* fileToSend;

			flag = 0;

			fileToSend = strtok(NULL, " \n");

			if(strtok(NULL, " \n") != NULL || fileToSend == NULL){
				printf("Invalid Command\n");
				flag = 1;
				continue;
			}

			if(flag == 1){
				continue;
			}

			flag = 0;

			pthread_mutex_lock(&lock);

			at = files.nextNode;

			while(at!=NULL){
				atValue = at->value;

				if(strcmp(atValue->nameOfFile, fileToSend) == 0){
					flag = 1;
					break;
				}

				at = at->nextNode;
			}

			pthread_mutex_unlock(&lock);

			if(flag == 0){
				printf("File not found on folder specified\n");
				continue;
			}

			sendInt(OPTION_SEND2, mysocket);

			len = strlen(fileToSend);

			sendInt(len, mysocket);

			sendString(fileToSend, len, mysocket);

			printf("File address sent succesfully\n");

			//atValue->onTheServer = 1;

			//temp = OPTION_SEND;
			//tempVoid = &temp;

			//sendVoid(tempVoid, sizeof(int), mysocket);
		}else if(strcmp(token, "erase")==0){
			int idOfFile, len;
			char* fileToErase;
			char* nameOfFileErased;			

			flag = 0;

			fileToErase = strtok(NULL, " \n");

			if(strtok(NULL, " \n") != NULL || fileToErase == NULL){
				printf("Invalid Command\n");
				flag = 1;
				continue;
			}

			if(flag == 1){
				continue;
			}

			idOfFile = 0;

			len = strlen(fileToErase);

			len--;

			for(i=len; i>=0; i--){
				idOfFile += (fileToErase[i] - '0') * pow(10, len - i);
			}

			sendInt(OPTION_ERASE, mysocket);

			sendInt(idOfFile, mysocket);

			len = recvInt(mysocket);

			if(len == -1){
				printf("File not found on server\n");
				continue;
			}

			nameOfFileErased = recvString(len, mysocket);

			len = recvInt(mysocket);
			char* ipOriginOfFileErased = recvString(len, mysocket);

			printf("File %s erased succesfully\n", nameOfFileErased);

			struct sockaddr_in destFile;

            int fileSocket = socket(AF_INET, SOCK_STREAM, 0);

            memset(&destFile, 0, sizeof(destFile));                /* zero the struct */
            destFile.sin_family = AF_INET;
            destFile.sin_addr.s_addr = inet_addr(ipOriginOfFileErased); //htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
            destFile.sin_port = htons( atoi( argv[2] ) );                /* set destination port number */

            int connectResultFile = connect(fileSocket, (struct sockaddr *)&destFile, sizeof(struct sockaddr_in));

            if( connectResultFile == - 1 ){
                continue;
            }

            sendInt(2, fileSocket);

            len = strlen(nameOfFileErased);

            sendInt(len, fileSocket);
            sendString(nameOfFileErased, len, fileSocket);

            close(fileSocket);
		}else if(strcmp(token, "Histrory")==0){
			//printf("Comando: %s\n", token);
			//printf("Atributos:\n");

			//imprimeLista(&historico);
			//printAtributes(token);
		}else if(strcmp(token, "help")==0){
			printf("The commands availables on this program are:\n");

		}else if(strcmp(token, "exit")==0){
			sendInt(OPTION_EXIT, mysocket);
			return EXIT_SUCCESS;
		}else{
			printf("Invalid Command\n");
		}
	}

	close(mysocket);

	return EXIT_SUCCESS;
}
