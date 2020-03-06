#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

struct Node;

typedef struct Node Node;

int hashFunction(void* value, int type);

void generateList(Node* node);

void addToList(Node * root, void* value);

int getSizeOfList(Node * root);

void generateHash(Node* hashTable);

void addToHash(Node* hashTable, void* value, int type);

#endif /* DATA_STRUCTURES_H */
