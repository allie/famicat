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
void Dictionary_Destroy(Dictionary* dictionary);
void Dictionary_Add(Dictionary* dictionary, const char* key, void* value);
void Dicitonary_Remove(Dictionary* dictionary, const char* key);
void* Dictionary_Get(Dictionary* dictionary, const char* key);

#endif
