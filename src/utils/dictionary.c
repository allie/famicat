#include <string.h>
#include "utils/dictionary.h"

/* TODO: This is a very simple implementation of a dictionary structure
         that naively searches through an array.  This needs to be replaced
         with an implementation utilizing a hash table.
*/

static int Dictionary_GetFreeId(Dictionary* dictionary) {
	if (dictionary->size == dictionary->capacity) {

		dictionary->entries = (Entry**)realloc(dictionary->entries,
			dictionary->capacity * 2 * sizeof(Entry*));

		memset(dictionary->entries + (dictionary->capacity * sizeof(Entry*)),
			0, dictionary->capacity * sizeof(Entry*));

		dictionary->capacity *= 2;
		return dictionary->size;
	}

	int id = 0;
	while (dictionary->entries[id] != NULL)
		id++;

	return id;
}

static int Dictionary_GetIndex(Dictionary* dictionary, const char* key) {
	if (dictionary->size == 0)
		return -1;

	int c = 0;
	int i;

	for (i = 0; i < dictionary->capacity && c < dictionary->size; i++) {
		if (dictionary->entries[i] == NULL)
			continue;

		if (strcmp(dictionary->entries[i]->key, key) == 0)
			return i;

		c++;
	}

	return -1;
}

Dictionary* Dictionary_New() {
	Dictionary* dictionary = (Dictionary*)malloc(sizeof(Dictionary));

	dictionary->size = 0;
	dictionary->capacity = INITIAL_DICTIONARY_CAPACITY;
	dictionary->entries = (Entry**)calloc(INITIAL_DICTIONARY_CAPACITY, sizeof(Entry*));

	return dictionary;
}

void Dictionary_Destroy(Dictionary* dictionary) {
	if (dictionary == NULL)
		return;

	int i;

	for (i = 0; i < dictionary->capacity; i++)
		free(dictionary->entries[i]);

	free(dictionary);
}

void Dictionary_Add(Dictionary* dictionary, const char* key, void* value) {
	if (dictionary == NULL)
		return;

	Entry* entry = (Entry*)malloc(sizeof(Entry));

	entry->key = key;
	entry->value = value;

	int id = Dictionary_GetFreeId(dictionary);

	dictionary->entries[id] = entry;
	dictionary->size++;
}

void Dictionary_Remove(Dictionary* dictionary, const char* key) {
	if (dictionary == NULL)
		return;

	int id = Dictionary_GetIndex(dictionary, key);

	if (id == -1)
		return;

	free(dictionary->entries[id]);

	dictionary->entries[id] = NULL;
	dictionary->size--;
}

void* Dictionary_Get(Dictionary* dictionary, const char* key) {
	if (dictionary == NULL)
		return NULL;

	int id = Dictionary_GetIndex(dictionary, key);

	if (id == -1 || dictionary->entries[id] == NULL)
		return NULL;

	return dictionary->entries[id]->value;
}
