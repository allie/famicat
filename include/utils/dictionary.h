#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "../common.h"

#define INITIAL_DICTIONARY_CAPACITY 128

typedef struct {
	const char* key;
	void* value;
} Entry;

typedef struct {
	int size;
	int capacity;
	Entry** entries;
} Dictionary;

Dictionary* Dictionary_New();
void Dictionary_Destroy(Dictionary*);
void Dictionary_Add(Dictionary*, const char*, void*);
void Dicitonary_Remove(Dictionary*, const char*);
void* Dictionary_Get(Dictionary*, const char*);
Entry** Dictionary_GetAll(Dictionary*, int*);
int Dictionary_Count(Dictionary*);

#endif
