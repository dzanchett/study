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

#include "dataStructures.h"

#define TYPE_INT 1
#define TYPE_CHAR 2

struct Node{
	void* value;
	struct Node * nextNode;
};

typedef struct Node Node;

int hashFunction(void* value, int type){
	switch(type){
		case TYPE_INT:
			;
			int* valueInt;
			valueInt = (int*) value;

			return (*valueInt)%1000;
			break;
		case TYPE_CHAR:
			;
			char* valueChar;
			valueChar = (char*) value;

			return (*valueChar)%1000;
			break;
	}
}

void generateList(Node* node){
	//node = (Node*) malloc(sizeof Node);

	node->nextNode = NULL;

	return;
}

void addToList(Node * root, void* value){
	Node * at;

	at = root;

	while(at->nextNode != NULL){
		at = at->nextNode;
	}

	at->nextNode = (Node*) malloc(sizeof(Node));

	at = at->nextNode;

	at->value = value;
	at->nextNode = NULL;

	return;
}

int getSizeOfList(Node * root){
	int size;
	Node* at;

	size = 0;

	at = root->nextNode;

	while(at!=NULL){
		size++;

		at = at->nextNode;
	}

	return size;
}

void generateHash(Node* hashTable){
	int i;

	hashTable = (Node*) malloc(sizeof(Node) * 1000);

	for(i=0; i<1000; i++){
		hashTable[i].nextNode = NULL;
	}
}

void addToHash(Node* hashTable, void* value, int type){
	int hash, i;

	hash = hashFunction(value, type);

	addToList(hashTable + hash, value);
}
