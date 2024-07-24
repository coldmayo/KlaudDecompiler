PUSH EBP
XOR edx,edx
MOV ebp,esp
MOV eax,DWORD PTR [ebp+0x8]
OR BYTE PTR [esi-0x75],dl
MOV esi,DWORD PTR [ebp+0xc]
OR ,0x53
LEA ebx,[eax-0x1]
INC DWORD PTR [edi]
MOV dh,0xc
PUSH SS
MOV BYTE PTR [ebx+edx*1+0x13],cl
ADC eax,DWORD PTR [ecx]
ADC edx,0x1
TEST cl,cl
JNZ 0x16
POP EBX
POP ESI
POP EBP
RET 
