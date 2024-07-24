#include <stdio.h>

static char reg_8[][10] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
static char reg_16[][10] = {"ax", "cx", "dx", "bx", "ax", "cx", "dx", "bx"};
static char reg_32[][10] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
static char sib_base[][10] = {"[eax", "[ecx", "[edx", "[ebx", "[esp", "[ebp", "[esi", "[edi"};
static char sib_scale[][10] = {"*1", "*2", "*4", "*8"};
