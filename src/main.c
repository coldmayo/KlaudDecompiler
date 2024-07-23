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

	for (int i = 0; i < 100 && i < file_size; i++) {
		printf("0x%02X ", buffer[i]);
		uint8_t byte = buffer[i];
		bytes[i] = byte;
	}
	printf("%d\n", bytes[0]);
	free(buffer);

	char out[0xFF];

	for (int i = 0, count = 0; i < sizeof(bytes); i+= count) {
		count = disassemble(bytes + i, sizeof(bytes) - i, i, out);
		printf("%s\n", out);
	}
}

int main() {
	read_bin("../test/test1");
}
