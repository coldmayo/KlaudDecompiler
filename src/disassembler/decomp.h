#include <stdint.h>
#include <stdbool.h>

#define LOCK 0xF0
#define REPNZ 0xF2
#define REP 0xF3
#define FS 0x64
#define GS 0x65

#include "opcode_tables.h"

typedef struct {
	bool lock;
	bool repnz;
	bool rep;
	bool gs;
	bool fs;
} prefix_info;

void modrm(uint8_t byte, uint8_t * stuff);
unsigned int disassemble(uint8_t * bytes, int max, int offset, char * output);
