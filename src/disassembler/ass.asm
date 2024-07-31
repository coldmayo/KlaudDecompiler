PUSH EBP
XOR edx,edx
MOV ebp,esp
MOV eax,DWORD PTR [ebp+0x8]
PUSH ESI
MOV esi,DWORD PTR [ebp+0xc]
PUSH EBX
LEA ebx,[eax-0x1]
MOVZX cl,BYTE PTR [esi+edx*1]
MOV BYTE PTR [ebx+edx*1+0x1],cl
ADC edx,0x1
TEST cl,cl
JNE 0x16
POP EBX
POP ESI
POP EBP
RET 
