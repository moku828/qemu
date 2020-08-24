.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x700083f1, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	CLRMAC
	/* t2.stepi() */
	
	MOV.L   immdata_0x00000000, R0
	/* t2.stepi() */
	
	LDS     R0, MACH
	/* t2.stepi() */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	
	LDS     R0, MACL
	/* t2.stepi() */
	
	MOV.L   _tblm, R1
	/* t2.stepi() */
	
	MOV.L   _tbln, R0
	/* t2.stepi() */
	
	MAC.W   @R0+, @R1+
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa000004c","r1":"0xa000004a","mach":"0x0","macl":"0x80000245","pc":"0xa0000014"}) */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	CLRMAC
	/* t2.stepi() */
	
	MOV.L   immdata_0x00000000, R0
	/* t2.stepi() */
	
	LDS     R0, MACH
	/* t2.stepi() */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	
	LDS     R0, MACL
	/* t2.stepi() */
	
	MOV.L   _tblm, R1
	/* t2.stepi() */
	
	MOV.L   _tbln, R0
	/* t2.stepi() */
	
	MAC.W   @R0+, @R1+
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa000004c","r1":"0xa000004a","mach":"0x1","macl":"0x7fffffff","pc":"0xa0000028"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x700083f1:
	.long   0x700083F1
immdata_0x700083f3:
	.long   0x700083F3
immdata_0x7fffffff:
	.long   0x7FFFFFFF
immdata_0x00000000:
	.long   0x00000000
_tblm:
	.long   tblm
_tbln:
	.long   tbln
tblm:
	.word   0x0123
tbln:
	.word   0x0002
