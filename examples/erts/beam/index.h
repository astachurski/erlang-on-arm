/*
 * index.h
 *
 *  Created on: Mar 31, 2014
 *      Author: Studnicki
 */

#ifndef INDEX_H_
#define INDEX_H_

#include "hash.h"

#define INDEX_PAGE_SHIFT 10
#define INDEX_PAGE_SIZE (1 << INDEX_PAGE_SHIFT)
#define INDEX_PAGE_MASK ((1 << INDEX_PAGE_SIZE) - 1)

typedef struct {
	HashBucket bucket;
	int index;
} IndexSlot;

typedef struct {
 	Hash htable; // obj -> index
	int size; // allocated size
	int limit; // max size
	int entries; // number of entries
	IndexSlot*** seg_table; // index -> obj
} IndexTable;

IndexTable* erts_index_init(IndexTable*, char*, int, int, HashFunctions);
IndexSlot* index_put_entry(IndexTable*, void*);
int index_get(IndexTable*, void*);

inline IndexSlot* erts_index_lookup(IndexTable* t, uint32_t ix);
inline int index_put(IndexTable* t, void* obj);

#endif /* INDEX_H_ */