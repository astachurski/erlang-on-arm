/*
 * beam_load.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Studnicki
 */

#include "global.h"
#include "beam_load.h"

void erts_load(byte* code) {
	LoaderState *loader = pvPortMalloc(sizeof(LoaderState));
	loader->code_file = code;

	//Initialize binary file and verify if it's ok
	if(init_iff_file(loader) || scan_iff_file(loader) || verify_chunks(loader)) {
		goto load_error;
	}

	//Load atom table
	loader->file_left = loader->chunks[ATOM_CHUNK].size;
	loader->file_p = loader->chunks[ATOM_CHUNK].start;
	if(load_atom_table(loader)) {
		goto load_error;
	}

	//Everything should be fine at this point
	goto free_loader;

	load_error:
	vPrintString("error while loading module\n");

	free_loader:
	vPrintString("freeing loader\n");
	vPortFree(loader);
}


static int load_atom_table(LoaderState* loader) {
	// @todo load the actual atom table
	// @todo check whether the first atom in the table is also name of the module
	uint8_t i;
	GetInt(loader, loader->num_atoms);
	char buf[256];

	// Read all atoms (indexes are 1..num_atoms in order to pick labeled atom directly)
	loader->num_atoms++;
	loader->atom = pvPortMalloc(loader->num_atoms*sizeof(Eterm));
	for(i=1; i<loader->num_atoms; i++) {
		byte* atom;
		uint8_t n;
		GetByte(loader, n);
		GetString(loader, atom, n);
		loader->atom[i] = erts_atom_put(atom, n);
	}

	// @todo check whether load->atom[1] is the module name, if so return an error
	return 0;
}

static int init_iff_file(LoaderState* loader) {
	uint32_t form_id = MakeIffId('F','O','R','1');
	byte* code = loader->code_file;
	if(MakeIffId(code[0],code[1],code[2],code[3])!=form_id) {
		vPrintString("Not a BEAM file, no FOR1 chunk\n");
		return 1;
	}

	// prepare LoaderState struct
	loader->file_p = code+4;
	//read the code size, in file it's big-endian, here it's most likely little-endian
	GetInt(loader, loader->code_size);
	loader->file_left = loader->code_size;

	// check file header, should be "BEAM"
	uint32_t beam_header = MakeIffId('B','E','A','M');
	uint32_t header;
	GetInt(loader, header);
	if(beam_header!=header) {
		vPrintString("Not a BEAM file, no BEAM header\n");
		return 1;
	}

	return 0;
}

static int scan_iff_file(LoaderState* loader) {

	// initialize chunks array
	uint8_t i;
	for(i=0; i<NUM_CHUNK_TYPES; i++) {
		loader->chunks[i].start = NULL;
		loader->chunks[i].size = 0;
	}

	// scan the file and read all the chunks
	while(loader->file_left) {
		uint32_t chunk_id, chunk_size;
		GetInt(loader, chunk_id);
		GetInt(loader, chunk_size);
		// @todo verify whether chunk_id is 4 ascii characters
		// @todo verify whether chunk_size is in bounds of what is left in the file

		// iterate over available chunks and check if the current one is one of these
		// observation: chunks may be in a random order in the BEAM file
		for(i=0; i<NUM_CHUNK_TYPES; i++) {
			if(chunk_types[i] == chunk_id) {
				loader->chunks[i].start = loader->file_p;
				loader->chunks[i].size = chunk_size;
				break;
			}
		}

		// go to the next chunk (the chunk area is padded to 4 bytes)
		uint8_t remainder = chunk_size % 4;
		chunk_size += (remainder == 0 ? 0 : (4-remainder));
		loader->file_p += chunk_size;
		loader->file_left -= chunk_size;
	}
	return 0;
}

static int verify_chunks(LoaderState* loader) {
	// @todo calculate md5 of file

	//check whether all mandatory chunks are present
	uint8_t i;
	for(i=0; i<NUM_MANDATORY; i++) {
		if(loader->chunks[i].start == NULL) {
			char buffer[33], cbuffer[5];
			cbuffer[4] = '\0';
			strncpy(cbuffer, &chunk_types[i], 4);
			sprintf(buffer, "No mandatory chunk of type %s\n", cbuffer);
			vPrintString(buffer);
			return 1;
		}
	}
	return 0;
}