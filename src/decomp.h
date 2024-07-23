#include <stdint.h>
#include <stdbool.h>

#include "opcode_tables.h"

void modrm(uint8_t byte, uint8_t * stuff);
unsigned int disassemble(uint8_t * bytes, int max, int offset, char * output);
