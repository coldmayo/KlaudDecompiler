#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "decomp.h"

void save_to_file(int i, char * code) {
	FILE * fptr;
	if (i == 0) {
		fptr = fopen("ass.asm", "w");
	} else {
		fptr = fopen("ass.asm", "a");
	}

	fprintf(fptr, code);

	fclose(fptr);
}

void read_bin(const char * pth) {
	FILE * file = fopen(pth, "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t * buffer = malloc(file_size);

	fread(buffer, 1, file_size, file);
	fclose(file);
	
	uint8_t bytes[] = {
    	0x55, 0x31, 0xD2, 0x89, 0xE5, 0x8B, 0x45, 0x08, 0x56, 0x8B, 0x75, 0x0C, 0x53, 0x8D, 0x58, 0xFF, 0x0F, 0xB6, 0x0C, 0x16, 0x88, 0x4C, 0x13, 0x01, 0x83, 0xC2, 0x01, 0x84, 0xC9, 0x75, 0xF1, 0x5B, 0x5E, 0x5D, 0xC3
	};

	char out[0xFF];
	char instruction[0xFF];

	for (int i = 0, count = 0; i < sizeof(bytes); i+= count) {
		count = disassemble(bytes + i, sizeof(bytes) - i, i, out);
		instruction[0] = 0;
		
		for (int e = 0; e < count; e++) {
			sprintf(instruction + strlen(instruction), "%02x ", bytes[i + e]);
		}
		
		printf("%-20s %s\n", instruction, out);
		save_to_file(i, out);
	}
	free(buffer);
}

int main(int argc, char * argv[]) {
	if (argc != 2) {
		read_bin("../../test/test1");
	} else {
		read_bin(argv[1]);
	}
}
