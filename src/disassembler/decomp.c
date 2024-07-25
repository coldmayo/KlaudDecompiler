#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "decomp.h"
#include "regs.h"

void modrm(uint8_t byte, uint8_t * stuff) {
	stuff[0] = (byte >> 6) & 0x3;
	stuff[1] = (byte >> 3) & 0x7;
	stuff[2] = byte & 0x7;
}

void SIB(uint8_t byte, char * stuff) {
	stuff[0] = ((byte) >> 6) & 0b11;
	stuff[1] = ((byte) >> 3) & 0b111;
	stuff[2] = (byte++) & 0b111;
}

void handle_prefix(uint8_t * bytes, prefix_info * pre) {
	while (true) {
		switch(*bytes) {
			case LOCK:
    			pre->lock = true;
    			bytes++;
    			break;
    		case REPNZ:
        		pre->repnz = true;
        		bytes++;
        		break;
        	case REP:
            	pre->rep = true;
            	bytes++;
            	break;
            case FS:
                pre->fs = true;
                bytes++;
                break;
            case GS:
                pre->gs = true;
                bytes++;
                break;
    		default:
        		return;
		}
	}
	
}

unsigned int disassemble(uint8_t * bytes, int max, int offset, char * output) {
	
	uint8_t illegals[] = {0x0F, 0xA6, 0xA7, 0xF7, 0xFF, 0x00};
	bool found = false;

	unsigned char * base = bytes;
	unsigned char opcode = *bytes++;

	char RM_output[256] = {0};
	char R_output[256] = {0};

	Instruction * instruction = one_byte;

	prefix_info pre = {
    	false, false, false, false, false
	};
	
	handle_prefix(bytes, &pre);	

	// deal with two byte opcodes or illegal opcodes
	if (opcode == 0x0F) {
		int i = 0;
		while (illegals[i] != 0x00) {
			if (illegals == bytes) {
				printf("illegal\n");
				return 1;
			} else if (max < 2) {
				printf("illegal\n");
				return 1;
			}
			i++;
		}
		instruction = two_byte;
		opcode = *bytes++;
		if (opcode == 0) {
			printf("illegal\n");
			return 6969;
		}
	}

	size_t instSize = sizeof(one_byte)/sizeof(Instruction);
	
	for (int i = 0; i < instSize; i++) {
		if (instruction[i].opcode == opcode) {
			//printf("Opcodes: 0x%x, 0x%x\n", instruction[i].opcode, opcode);
			found = true;
			strcpy(output, instruction[i].asmb);

			uint8_t mod, reg, rm;
			// dealing with ecodings (SIB and MODR/M)
			if (instruction[i].hasModRM) {

				uint8_t stuff[3] = {0x0, 0x0, 0x0};
				modrm(*bytes++, stuff);
				mod = stuff[0];
				reg = stuff[1];
				rm = stuff[2];

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
    					char scale, index, base;
    					char stuff[3] = {'a'};
    					SIB(*bytes, stuff);
						scale = stuff[0];
						index = stuff[1];
						base = stuff[2];

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
			
			char temp[256];
            strcpy(temp, "");

            if (pre.lock) {
                strcat(temp, "LOCK ");
            }
            if (pre.repnz) {
                strcat(temp, "REPNZ ");
            }
            if (pre.rep) {
                strcat(temp, "REP ");
            }
            if (pre.fs) {
				strcat(temp, "FS ");
            }
            if (pre.gs) {
				strcat(temp, "GS ");
            }

            strcat(temp, instruction[i].asmb);
            strcpy(output, temp);
			
			for (int j = 0; j < instruction[i].arg_count; j++) {
				if (j > 0) {
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
            			sprintf(output + strlen(output), "0x%lu", offset + ((bytes - base) + 1) + *(char *)bytes++);
            			break;
				}
			}
		}
	}

	//if (!found) {
		//sprintf(output + strlen(output), "0x%x", *bytes++);
	//}

	strcat(output, "\n");
	
	return bytes - base;
}
