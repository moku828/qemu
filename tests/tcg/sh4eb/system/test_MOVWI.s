.global test

.section .text, "ax"

test:
	MOV.W   immdata_0x7fff, R0
	MOV.W   immdata_0x8000, R1
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
