/*
 * atom.c
 *
 *  Created on: Mar 31, 2014
 *      Author: Studnicki
 */

#include "global.h"
#include "atom.h"

IndexTable erts_atom_table;

void dump_atoms() {
	int i = erts_atom_table.entries;

	debug("atom dump start\n");
	while(--i >= 0) {
		if(erts_index_lookup(&erts_atom_table, i)) {
			debug("atom: ");
			debug_32(i);
		}
	}
	debug("atom dump finished\n");
}

Eterm erts_atom_put(const byte* name, int len) {
	Atom a;
	int aix;

	a.len = (uint16_t)len;
	a.name = pvPortMalloc(len);
	memcpy(a.name, name, len);

	aix = index_get(&erts_atom_table, (void*)&a);
	if(aix >= 0) {
		return make_atom(aix);
	}

	aix = index_put(&erts_atom_table, (void*)&a);
	Eterm atom = make_atom(aix);

	return atom;
}

int erts_atom_get(const char* name, int len, Eterm* ap) {
	Atom a;
	int i;
	int res;

	a.len = (uint16_t)len;
	a.name = (byte*)name;
	i = index_get(&erts_atom_table, (void*)&a);
	res = i < 0 ? 0 : (*ap = make_atom(i), 1);

	return res;
}

void init_atom_table(void) {
	HashFunctions f;
	f.hash = (H_FUN)atom_hash;
	f.cmp = (HCMP_FUN)atom_cmp;
	f.alloc = (HALLOC_FUN)atom_alloc;
	f.free = (HFREE_FUN)atom_free;

	erts_index_init(&erts_atom_table, "atom_tab", 1024, 2048, f);
}

// Hash-table callbacks
// hashpjw
static HashValue atom_hash(Atom* obj) {
	 byte* p = obj->name;
	 int len = obj->len;
	 HashValue h = 0, g;

	 while(len--) {
		h = (h << 4) + *p++;
		if ((g = h & 0xf0000000)) {
		    h ^= (g >> 24);
		    h ^= g;
		}
	 }
	 return h;
}

static int atom_cmp(Atom* tmp, Atom* obj) {
	if(tmp->len == obj->len &&
	   memcmp(tmp->name, obj->name, tmp->len) == 0) {
		return 0;
	}
	return 1;
}

static Atom* atom_alloc(Atom* tmp) {
	Atom* obj = (Atom*)pvPortMalloc(sizeof(Atom));
	obj->name = (byte*)pvPortMalloc(tmp->len);
	memcpy(obj->name, tmp->name, tmp->len);
	obj->len = tmp->len;
	obj->slot.index = -1;

	//calculate ord (used for atom comparision)
	uint8_t c[4];
	int i,j;
	j = (tmp->len < 4) ? tmp->len : 4;
	for(i=0; i<j; i++) {
		c[i] = tmp->name[i];
	}
	for(; i<4; i++) {
		c[i] = '\0';
	}
	obj->ord0 = (c[0] << 23) + (c[1] << 15) + (c[2] << 7) + (c[3] >> 1);

	return obj;
}

static void atom_free(Atom* obj) {
	vPortFree((void*)obj);
}