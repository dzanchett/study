#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define OPTION_LIST 1
#define OPTION_FIND 2
#define OPTION_SEND 3
#define OPTION_ERASE 4
#define OPTION_EXIT 5
#define OPTION_SEND2 6

struct File{
    int id;
    char* name;
    char* ipOrigin;
    int sent;
};

struct ThreadArgument_t{
    int consocket;
    struct sockaddr_in dest;
};

struct Node{
    void* value;
    struct Node * nextNode;
};

typedef struct File File;
typedef struct ThreadArgument_t ThreadArgument;
typedef struct Node Node;

int idFileCount;
char* portUsed;
Node files;

pthread_mutex_t lock;

void* threadSocket( void *ptr ){
    int flagLoop, consocket, len, i, j;
    struct sockaddr_in dest;
    Node* at;
    Node* ant;

    ThreadArgument* arg = (ThreadArgument*) ptr;

    consocket = arg->consocket;
    dest = arg->dest;

    flagLoop = 0;

        while(TRUE){
            int option = recvInt(consocket);

            File* atValue;

            switch(option){
                case OPTION_LIST:
                    len = strlen(inet_ntoa(dest.sin_addr)) * sizeof(char);
                    sendInt(len, consocket);
                    sendString(inet_ntoa(dest.sin_addr), len, consocket);

                    sendInt(getSizeOfList(&files), consocket);

                    at = files.nextNode;

                    while(at!=NULL){
                        atValue = (File*) at->value;

                        sendInt(atValue->id, consocket);

                        len = strlen(atValue->name) * sizeof(char);
                        sendInt(len, consocket);
                        sendString(atValue->name, len, consocket);

                        len = strlen(atValue->ipOrigin) * sizeof(char);
                        sendInt(len, consocket);
                        sendString(atValue->ipOrigin, len, consocket);

                        sendInt(atValue->sent, consocket);

                        at = at->nextNode;
                    }

                    break;
                case OPTION_FIND:
                    ;
                    int idOfFile = recvInt(consocket);

                    pthread_mutex_lock(&lock);

                    at = files.nextNode;

                    while(at!=NULL){
                        atValue = (File*) at->value;

                        if(atValue->id == idOfFile){
                            atValue->sent = 0;
                            break;
                        }

                        at = at->nextNode;
                    }

                    if(at==NULL){
                        sendInt(-1, consocket);
                        break;
                    }

                    pthread_mutex_unlock(&lock);

                    sendInt(1, consocket);
                    
                    len = strlen(atValue->ipOrigin);
                    sendInt(len, consocket);
                    sendString(atValue->ipOrigin, len, consocket);

                    len = strlen(atValue->name);
                    sendInt(len, consocket);
                    sendString(atValue->name, len, consocket);

                    break;
                case OPTION_SEND:
                    ;
                    int sizeNameOfFile;

                    sizeNameOfFile = recvInt(consocket);

                    char* nameOfFile;

                    nameOfFile = (char*) recvString(sizeNameOfFile, consocket);

                    atValue = (File*) malloc(sizeof(File));

                    atValue->id = idFileCount;
                    atValue->name = nameOfFile;

                    atValue->ipOrigin = (char*) malloc(strlen(inet_ntoa(dest.sin_addr)) + 1);
                    strcpy(atValue->ipOrigin, inet_ntoa(dest.sin_addr));

                    atValue->sent = 0;

                    pthread_mutex_lock(&lock);

                    at = files.nextNode;

                    File* tempFile;

                    tempFile = atValue;

                    while(at!=NULL){
                        atValue = (File*) at->value;

                        if(strcmp(tempFile->name, atValue->name) == 0 && strcmp(tempFile->ipOrigin, atValue->ipOrigin) == 0){
                            continue;
                        }

                        at = at->nextNode;
                    }

                    printf("File Received:\n");
                    printf("\tId of File: %d\n", getSizeOfList(&files));
                    printf("\tName of File: %s\n", nameOfFile);
                    printf("\tIP of Origin: %s\n", inet_ntoa(dest.sin_addr));

                    addToList(&files, (void*) tempFile);
                    idFileCount++;

                    pthread_mutex_unlock(&lock);

                    break;
                case OPTION_SEND2:
                    ;
                    int lenSent;
                    char* nameOfFileSent;

                    lenSent = recvInt(consocket);
                    nameOfFileSent = recvString(lenSent, consocket);

                    pthread_mutex_lock(&lock);

                    at = files.nextNode;

                    while(at!=NULL){
                        atValue = at->value;

                        if(strcmp(atValue->name, nameOfFileSent) == 0 && strcmp(atValue->ipOrigin, inet_ntoa(dest.sin_addr)) == 0){
                            atValue->sent = 1;
                        }

                        at = at->nextNode;
                    }

                    pthread_mutex_unlock(&lock);
                    break;
                case OPTION_ERASE:
                    ;
                    int idOfFileToErase, len;
                    char* nameOfFileToErase;
                    char* ipOriginOfFileErased;

                    nameOfFileToErase = NULL;

                    idOfFileToErase = recvInt(consocket);

                    pthread_mutex_lock(&lock);

                    at = files.nextNode;
                    ant = &files;

                    while(at!=NULL){
                        atValue = at->value;

                        if(atValue->id == idOfFileToErase){
                            nameOfFileToErase = (char*) malloc(strlen(atValue->name) * sizeof(char));
                            strcpy(nameOfFileToErase, atValue->name);

                            ipOriginOfFileErased = (char*) malloc(strlen(atValue->ipOrigin) * sizeof(char));
                            strcpy(ipOriginOfFileErased, atValue->ipOrigin);

                            ant->nextNode = at->nextNode;

                            free(at);
                        }else{
                            ant = at;
                        }

                        at = at->nextNode;
                    }

                    pthread_mutex_unlock(&lock);

                    if(nameOfFileToErase == NULL){
                        sendInt(-1, consocket);
                        break;
                    }

                    len = strlen(nameOfFileToErase);

                    sendInt(len, consocket);
                    sendString(nameOfFileToErase, len, consocket);

                    len = strlen(ipOriginOfFileErased);

                    sendInt(len, consocket);
                    sendString(ipOriginOfFileErased, len, consocket);
                    break;
                case OPTION_EXIT:
                    close(consocket);
                    flagLoop = 1;

                    pthread_mutex_lock(&lock);

                    at = files.nextNode;
                    ant = &files;

                    while(at!=NULL){
                        atValue = (File*) at->value;

                        if(strcmp(atValue->ipOrigin, inet_ntoa(dest.sin_addr)) == 0){
                            ant->nextNode = at->nextNode;

                            free(((File*)at->value)->name);
                            free(((File*)at->value)->ipOrigin);
                            free(at);
                        }else{
                            ant = at;
                        }

                        at = at->nextNode;
                    }

                    pthread_mutex_unlock(&lock);

                    break;
            }

            if(flagLoop == 1){
                break;
            }
        }
}

int main(int argc, char *argv[]){
    int i, j, len;
    Node irets;
    Node threads;
    Node* at;
    Node* ant;

    pthread_mutex_init(&lock, NULL);

    idFileCount = 0;

    generateList(&files);
    generateList(&irets);
    generateList(&threads);

    if( argc != 2 ){ 
        printf("USAGE: server port_number\n");

        return EXIT_FAILURE;
    }

    portUsed = argv[2];

    srand( time( NULL ));

    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);

    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons( atoi( argv[1] ));           /* set the server port number */

    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

    /* start listening, allowing a queue of up to 1 pending connection */
    listen(mysocket, 1);

    printf("Server is waiting for connections on port:%s\n", argv[1] );

    //printf("%s\n", generateString(TAM_STRING));

    int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

    while(consocket){
        printf("Incoming connection from %s\n", inet_ntoa(dest.sin_addr));

        ThreadArgument arg;

        arg.consocket = consocket;
        arg.dest = dest;

        int* tempInt = (int*) malloc(sizeof(int));

        pthread_t* tempThread = (pthread_t*) malloc(sizeof(pthread_t));

        *tempInt = pthread_create(tempThread, NULL, threadSocket, (void*) &arg);

        if(*tempInt){
            fprintf(stderr,"Error - pthread_create() return code: %d\n", *tempInt);
            exit( EXIT_FAILURE );
        }

        addToList(&irets, (void*) tempInt);
        addToList(&threads, (void*) tempThread);

        consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
    }

    close(mysocket);

    return EXIT_SUCCESS;
}
