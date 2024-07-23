# How Decompilers Work

Decompilation is when a program written in machine code is translated into an equivalent program in a higher level language. It is impossible for the decompiler to 100% correctly decompile every program since the process has loss. For example variable names + comments cannot be extracted. The one I will be making is a C compiler for Linux machines.

## Phases

Starts with a binary program:

- Syntax Analyzer
- Semantic Analyzer
- Intermediate Code Generator
- Control Flow Graph Generator
- Data Flow Analyzer
- Control Flow Analyzer
- Code Generator

Then the High Level Language program should be outputted.

The first thing I will do is make a disassembler, which takes binary and turns it into machine code. This corresponds to the first 3 bullet points.

## Disassembler

First, we need to build an opcode table, this will translate from hex to the assembly instruction. In a perfect world we could iterate through bytes and translate them to the instruction but theres a lot more to it than that.

A good reference is: <a href="http://ref.x86asm.net/coder32.html">here</a>

Definitions:
- Opcode: portion of a machine language instruction that specifies the operation to be preformed.
- ModR/M is used for encoding a register or (in our case) opcode extention.
	- The byte has 3 feilds: mod, reg, and rm
		- mod (2 bits): when this is b11 then register-direct addressing is used, otherwise register-indirect addressing is used
		- reg (3 bits): can be used either to distingiush between other opcodes or used as the source or the destination of an instruction
		- rm (3 bits): specifies a direct or indirect register operand, possibly with a displacement
- SIB is similar to ModR/M in that it is used for encoding purposes
	- The byte has 3 feilds: scale, index, base
		- scale (2 bits): indicates the scaling factor for index, where $s$ equals $2^{scale}$:
			- scale: b00, factor ($s$): 1
			- scale: b01, factor ($s$): 2
			- scale: b10, factor ($s$): 4
			- scale: b11, factor ($s$): 8
		- index (3 bits): shows which register to use
		- base (3 bits): shows which base register to use

In this opcode table we need to build a table that shows if modr/m is used for this byte, the size of the output of the command (DWORD [unsigned 32 bit value], WORD [unsigned 16 bit value], BYTE [unsigned 8 bit value]), and the arguments.

Opcodes have 2 classifications: one-byte (0x00 to 0xFF) and two-byte (0x0F to 0x0FFF)

## Decompiling

# Sources
1. <a href = "https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR/M_and_SIB_bytes">OSDev.org</a>
2. <a href = "https://www.scs.stanford.edu/05au-cs240c/lab/i386/c17.htm">80386 Instruction Set</a>