#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "decomp.h"

void read_bin(const char * pth) {
	FILE * file = fopen(pth, "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char * buffer = malloc(file_size);

	fread(buffer, 1, file_size, file);
	fclose(file);

	uint8_t bytes[256] = {0};
	uint8_t bytes2[256] = {
    	0x55, 0x31, 0xD2, 0x89, 0xE5, 0x8B, 0x45, 0x08, 0x56, 0x8B, 0x75, 0x0C, 0x53, 0x8D, 0x58, 0xFF, 0x0F, 0xB6, 0x0C, 0x16, 0x88, 0x4C, 0x13, 0x01, 0x83, 0xC2, 0x01, 0x84, 0xC9, 0x75, 0xF1, 0x5B, 0x5E, 0x5D, 0xC3
	};
	for (int i = 0; i < 100 && i < file_size; i++) {
		//printf("0x%02X ", (unsigned char)buffer[i]);
		uint8_t byte = (uint8_t)buffer[i];
		bytes[i] = byte;
	}
	free(buffer);

	char out[256];

	for (int i = 0, count = 0; i < sizeof(bytes2); i+= count) {
		//printf("plebus\n");
		count = disassemble(bytes2 + i, sizeof(bytes2) - i, i, out);
		printf("%s\n", out);
	}
}

int main() {
	read_bin("../test/test1");
}
