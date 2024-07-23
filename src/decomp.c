#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "decomp.h"

enum {
	AL, EAX, ES, CS, SS, DS, ONE, CL, XMM0, BND0, BAD, MM0,
	IMM8, IMM16, IMM32, REL8, REL32, PTR1632, R, RM,
	BYTE, WORD, DWORD, QWORD, FWORD, XMMWORD
};

typedef struct {
	bool hasModRM;   //modr/m ?
	char size;   // 
	const char *asmb;   //mnemonic
	uint8_t opcode;
	char arg_count;
	char arguments[4];
} Instruction;

static Instruction one_byte[255] = {
    {true, BYTE, "ADD ", 0x00, 2, {RM, R}},
    {true, DWORD, "ADD ", 0x01, 2, {RM, R}},
    {true, BYTE, "ADD ", 0x02, 2, {R, RM}},
    {true, DWORD, "ADD ", 0x03, 2, {R, RM}},
    {false, 0, "ADD ", 0x04, 2, {AL, IMM8}},
    {false, 0, "ADD ", 0x05, 2, {EAX, IMM32}},
    {false, 0, "PUSH ES", 0x06, 0, {0}},
    {false, 0, "POP ES", 0x07, 0, {0}},
    {true, BYTE, "OR ", 0x08, 2, {RM, R}},
    {true, DWORD, "OR ", 0x09, 2, {RM, R}},
    {true, BYTE, "OR ", 0x0A, 2, {RM, R}},
    {true, DWORD, "OR ", 0x0B, 2, {RM, R}},
    {false, 0, "OR ", 0x0C, 2, {AL, IMM8}},
    {false, 0, "OR ", 0x0D, 2, {EAX, IMM32}},
    {false, 0, "POP CS", 0x0E, 0, {0}},
    {true, BYTE, "ADC ", 0x10, 2, {RM, R}},
    {true, DWORD, "ADC ", 0x11, 2, {RM, R}},
    {true, BYTE, "ADC ", 0x12, 2, {R, RM}},
    {true, DWORD, "ADC ", 0x13, 2, {R, RM}},
    {false, 0, "ADC ", 0x14, 2, {AL, IMM8}},
    {false, 0, "ADC ", 0x15, 2, {EAX, IMM32}},
    {false, 0, "PUSH SS", 0x16, 0, {0}},
    {false, 0, "POP SS", 0x17, 0, {0}},
    {true, BYTE, "SBB ", 0x18, 2, {RM, R}},
    {true, DWORD, "SBB ", 0x19, 2, {RM, R}},
    {true, BYTE, "SBB ", 0x1A, 2, {R, RM}},
    {true, DWORD, "SBB ", 0x1B, 2, {R, RM}},
    {false, 0, "SBB ", 0x1C, 2, {AL, IMM8}},
    {false, 0, "SBB ", 0x1D, 2, {EAX, IMM32}},
    {false, 0, "PUSH DS", 0x1E, 0, {0}},
    {false, 0, "POP DS", 0x1F, 0, {0}},
    {true, BYTE, "AND ", 0x20, 2, {RM, R}},
    {true, DWORD, "AND ", 0x21, 2, {RM, R}},
    {true, BYTE, "AND ", 0x22, 2, {R, RM}},
    {true, DWORD, "AND ", 0x23, 2, {R, RM}},
    {false, 0, "AND ", 0x24, 2, {AL, IMM8}},
    {false, 0, "AND ", 0x25, 2, {EAX, IMM32}},
    {false, 0, "ES ", 0x26, 0, {0}},
    {false, 0, "DAA ", 0x27, 0, {0}},
    {true, BYTE, "SUB ", 0x28, 2, {RM, R}},
    {true, DWORD, "SUB ", 0x29, 2, {RM, R}},
    {true, BYTE, "SUB ", 0x2A, 2, {R, RM}},
    {true, DWORD, "SUB ", 0x2B, 2, {R, RM}},
    {false, 0, "SUB ", 0x2C, 2, {AL, IMM8}},
    {false, 0, "SUB ", 0x2D, 2, {EAX, IMM32}},
    {false, 0, "CS ", 0x2E, 0, {0}},
    {false, 0, " DAS ", 0x2F, 0, {0}},
    {true, BYTE, "XOR ", 0x30, 2, {RM, R}},
    {true, DWORD, "XOR ", 0x31, 2, {RM, R}},
    {true, BYTE, "XOR ", 0x32, 2, {R, RM}},
    {true, DWORD, "XOR ", 0x33, 2, {R, RM}},
    {false, 0, "XOR ", 0x34, 2, {AL, IMM8}},
    {false, 0, "XOR ", 0x35, 2, {EAX, IMM32}},
    {false, 0, "SS ", 0x36, 0, {0}},
    {false, 0, "AAA ", 0x37, 0, {0}},
    {true, BYTE, "CMP ", 0x38, 2, {RM, R}},
    {true, DWORD, "CMP ", 0x39, 2, {RM, R}},
    {true, BYTE, "CMP ", 0x3A, 2, {R, RM}},
    {true, DWORD, "CMP ", 0x3B, 2, {R, RM}},
    {false, 0, "CMP ", 0x3C, 2, {AL, IMM8}},
    {false, 0, "CMP ", 0x3D, 2, {EAX, IMM32}},
    {false, 0, "DS ", 0x3E, 0, {0}},
    {false, 0, "AAS ", 0x3F, 0, {0}},
    {false, 0, "INC EAX", 0x40, 0, {0}},
    {false, 0, "INC ECX", 0x41, 0, {0}},
    {false, 0, "INC EDX", 0x42, 0, {0}},
    {false, 0, "INC EBX", 0x43, 0, {0}},
    {false, 0, "INC ESP", 0x44, 0, {0}},
    {false, 0, "INC EBP", 0x45, 0, {0}},
    {false, 0, "INC ESI", 0x46, 0, {0}},
    {false, 0, "INC EDI", 0x47, 0, {0}},
    {false, 0, "DEC EAX", 0x48, 0, {0}},
    {false, 0, "DEC ECX", 0x49, 0, {0}},
    {false, 0, "DEC EDX", 0x4A, 0, {0}},
    {false, 0, "DEC EBX", 0x4B, 0, {0}},
    {false, 0, "DEC ESP", 0x4C, 0, {0}},
    {false, 0, "DEC EBP", 0x4D, 0, {0}},
    {false, 0, "INC ESI", 0x4E, 0, {0}},
    {false, 0, "INC EDI", 0x4F, 0, {0}},
    {false, 0, "PUSH EAX", 0x50, 0, {0}},
    {false, 0, "PUSH ECX", 0x51, 0, {0}},
    {false, 0, "PUSH EDX", 0x52, 0, {0}},
    {false, 0, "PUSH EBX", 0x53, 0, {0}},
    {false, 0, "PUSH ESP", 0x54, 0, {0}},
    {false, 0, "PUSH EBP", 0x55, 0, {0}},
    {false, 0, "PUSH ESI", 0x56, 0, {0}},
    {false, 0, "PUSH EDI", 0x57, 0, {0}},
    {false, 0, "POP EAX", 0x58, 0, {0}},
    {false, 0, "POP ECX", 0x59, 0, {0}},
    {false, 0, "POP EDX", 0x5A, 0, {0}},
    {false, 0, "POP EBX", 0x5B, 0, {0}},
    {false, 0, "POP ESP", 0x5C, 0, {0}},
    {false, 0, "POP EBP", 0x5D, 0, {0}},
    {false, 0, "POP ESI", 0x5E, 0, {0}},
    {false, 0, "POP EDI", 0x5F, 0, {0}},
    {false, 0, "PUSHA", 0x60, 0, {0}},
    {false, 0, "POPA", 0x61, 0, {0}},
    {true, QWORD, "BOUND ", 0x62, 2, {R, RM}},
    {true, WORD, "ARPL ", 2, 0x63, {RM, R}}
};

static Instruction two_byte[255] = {
    {true, WORD, "sldt ", 1, {RM}},
    {true, 0, "sgdt ", 1, {RM}}
};

void modrm(uint8_t byte, uint8_t * stuff) {
	uint8_t mod = (byte >> 6) & 0x3;
	stuff[0] = mod;
	uint8_t reg = (byte >> 3) & 0x7;
	stuff[1] = reg;
	uint8_t rm = byte & 0x7;
	stuff[2] = rm;
}

unsigned int disassemble(uint8_t * bytes, int max, int offset, char * output) {

	static char reg_8[][10] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
	static char reg_16[][10] = {"ax", "cx", "dx", "bx", "ax", "cx", "dx", "bx"};
	static char reg_32[][10] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
	static char sib_base[][10] = {"[eax", "[ecx", "[edx", "[ebx", "[esp", "[ebp", "[esi", "[edi"};
	static char sib_scale[][10] = {"*1", "*2", "*4", "*8"};
	uint8_t illegals[] = {0x0F, 0xA6, 0xA7, 0xF7, 0xFF, 0x0};

	unsigned char * base = bytes;
	unsigned char opcode = *bytes++;

	char RM_output[0xFF] = {0};
	char R_output[0xFF] = {0};

	Instruction * instruction = one_byte;

	// for two-byte opcodes (i.e. starts with 0x0F)
	if (opcode == 0x0F) {
		int i = 0;
		while (illegals[i] != 0x0) {
			if (illegals == bytes) {
				printf("illegal");
				return 1;
			} else if (max < 2) {
				printf("illegal");
				return 1;
			}
			i++;
		}
		instruction = two_byte;
		opcode = *bytes++;
	}

	size_t instSize = sizeof(one_byte)/sizeof(Instruction);
	
	for (int i = 0; i < instSize; i++) {
		if (instruction[i].opcode == opcode) {
			printf("Opcodes: 0x%x, 0x%x\n", instruction[i].opcode, opcode);

			strcpy(output, instruction[i].asmb);

			uint8_t mod, reg, rm;
			
			if (instruction[i].hasModRM) {

				uint8_t stuff[3] = {0x0, 0x0, 0x0};
				modrm(*bytes++, stuff);
				uint8_t mod = stuff[0];
				uint8_t reg = stuff[1];
				uint8_t rm = stuff[2];

				switch (instruction[i].size) {
					case WORD:
    					strcpy(R_output, reg_16[reg]);
    					break;
    				case BYTE:
    					strcpy(R_output, reg_8[reg]);
    					break;
    				default:
        				strcpy(R_output, reg_32[reg]);
				}

				if (mod == 0b11) {
					switch (instruction[i].size) {
						case BYTE:
							sprintf(RM_output, "%s", reg_8[rm]);
							break;
						case WORD:
							sprintf(RM_output, "%s", reg_16[rm]);
							break;
						default:
    						sprintf(RM_output, "%s", reg_32[rm]);
					}
				} else if (mod == 0b00 && rm == 0b101) {
					sprintf(RM_output, "[0x%x]", *(int *)bytes);
					bytes += 4;
				} else {
					if (rm == 0b100) {
						char scale = ((*bytes) >> 6) & 0b11;
						char index = ((*bytes) >> 3) & 0b111;
						char base = (*bytes++) & 0b111;

						if (base == 0b101 && mod == 0b00) {
							sprintf(RM_output, "[0x%x", *(int *)bytes);
							bytes += 4;
						} else {
							strcpy(RM_output, sib_base[base]);
						}

						if (index != 0b100) {
							strcat(RM_output, "+");
							strcat(RM_output, reg_32[index]);
							strcat(RM_output, sib_scale[scale]);
						}
					} else {
						sprintf(RM_output, "[%s", reg_32[rm]);
					}

					if (mod == 0b01) {
						if (*bytes > 0x7F) {
							sprintf(RM_output + strlen(RM_output), "-0x%x]", -*(char *)bytes);
						} else {
							sprintf(RM_output + strlen(RM_output), "+0x%x]", *(char *)bytes);
						}
					} else if (mod == 0b10) {
						if (*(unsigned int *)bytes > 0x7FFFFFFF) {
    						sprintf(RM_output + strlen(RM_output), "-0x%x]", -*(int *)bytes);
						} else {
							sprintf(RM_output + strlen(RM_output), "+0x%x]", *(unsigned int *)bytes);
						}
						bytes += 4;
					} else {
						strcat(RM_output, "]");
					}
				}
				
			}
	
			for (int j = 0; j < instruction[i].arg_count; i++) {
				if (i > 0) {
					strcat(output, ",");
				}

				switch (instruction[i].arguments[j]) {
					case RM:
    					if (mod != 0b11) {
							switch (instruction[i].size) {
								case BYTE:
    								strcat(output, "BYTE PTR ");
    								break;
    							case WORD:
        							strcat(output, "WORD PTR ");
        							break;
        						case DWORD:
            						strcat(output, "DWORD PTR ");
            						break;
            					case QWORD:
                					strcat(output, "QWORD PTR ");
                					break;
                				case FWORD:
                    				strcat(output, "FWORD PTR ");
                    				break;
                    			case XMMWORD:
                        			strcat(output, "XMMWORD PTR ");
                        			break;
    						}
    					}
    					strcat(output, RM_output);
    					break;
    				case R:
        				strcat(output, R_output);
        				break;
        			case IMM8:
            			sprintf(output + strlen(output), "0x%x", *bytes++);
            			break;
    				case IMM16:
        				sprintf(output + strlen(output), "0x%x", *(short *)bytes);
        				break;
        			case IMM32:
            			sprintf(output + strlen(output), "0x%x", *(int *)bytes);
            			break;
            		case REL8:
            			sprintf(output + strlen(output), "0x%x", offset + ((bytes - base) + 1) + *(char *)bytes++);
            			break;
				}
			}
		}
	}
	
	return bytes - base;
}
