/*
 * atom.c
 *
 *  Created on: Mar 31, 2014
 *      Author: Studnicki
 */

#include "global.h"
#include "atom.h"
#include "io.h"
#include "atom_initial_table.h"

IndexTable erts_atom_table;

void dump_atoms() {
	int i = erts_atom_table.entries;

	char buf[30], buf_atom[50];
	debug("atom dump start\n");
	while(--i >= 0) {
		Atom* atom = (Atom*)erts_index_lookup(&erts_atom_table, i);
		if(erts_index_lookup(&erts_atom_table, i)) {
			strncpy(buf_atom, atom->name, atom->len);
			buf_atom[atom->len] = '\0';
			sprintf(buf, "atom %d %s\n", i, buf_atom);
			debug(buf);
		}
	}
	debug("atom dump finished\n");
}

Eterm erts_atom_put(const byte* name, int len) {
	Atom a;
	int aix;

	a.len = (uint8_t)len;
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

	a.len = (uint8_t)len;
	a.name = (byte*)name;
	i = index_get(&erts_atom_table, (void*)&a);

	if(i < 0) {
		return 0;
	}
	else {
		*ap = make_atom(i);
		return 1;
	}
}

void init_atom_table(void) {
	HashFunctions f;
	int i;
	f.hash = (H_FUN)atom_hash;
	f.cmp = (HCMP_FUN)atom_cmp;
	f.alloc = (HALLOC_FUN)atom_alloc;
	f.free = (HFREE_FUN)atom_free;
	erts_index_init(&erts_atom_table, "atom_tab", ATOM_TABLE_SIZE, ATOM_TABLE_SIZE, f);

	for(i=0; i<(sizeof(erl_atom_names)/sizeof(erl_atom_names[0])); i++) {
		erts_atom_put(erl_atom_names[i], strlen(erl_atom_names[i]));
	}
}

// Hash-table callbacks
// hashpjw
static HashValue atom_hash(Atom* obj) {
	 byte* p = obj->name;
	 int len = obj->len;
	 HashValue h = 0, g;

	 while(len--) {
		h = (h << 1) + *p++;
		if ((g = h & 0xf0)) {
		    h ^= (g >> 6);
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
	return obj;
}

static void atom_free(Atom* obj) {
	vPortFree((void*)obj);
}
