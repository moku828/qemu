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
	
	MOV.L   immdata_0x00007fff, R0
	/* t2.stepi() */
	
	LDS     R0, MACH
	/* t2.stepi() */
	
	MOV.L   immdata_0xffffffff, R0
	/* t2.stepi() */
	
	LDS     R0, MACL
	/* t2.stepi() */
	
	MOV.L   _tblm, R1
	/* t2.stepi() */
	
	MOV.L   _tbln, R0
	/* t2.stepi() */
	
	MAC.L   @R0+, @R1+
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa0000050","r1":"0xa000004c","mach":"0x8000","macl":"0x2468acd","pc":"0xa0000014"}) */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	CLRMAC
	/* t2.stepi() */
	
	MOV.L   immdata_0x00007fff, R0
	/* t2.stepi() */
	
	LDS     R0, MACH
	/* t2.stepi() */
	
	MOV.L   immdata_0xffffffff, R0
	/* t2.stepi() */
	
	LDS     R0, MACL
	/* t2.stepi() */
	
	MOV.L   _tblm, R1
	/* t2.stepi() */
	
	MOV.L   _tbln, R0
	/* t2.stepi() */
	
	MAC.L   @R0+, @R1+
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa0000050","r1":"0xa000004c","mach":"0x0","macl":"0x2468acd","pc":"0xa0000028"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x700083f1:
	.long   0x700083F1
immdata_0x700083f3:
	.long   0x700083F3
immdata_0xffffffff:
	.long   0xFFFFFFFF
immdata_0x00007fff:
	.long   0x00007FFF
_tblm:
	.long   tblm
_tbln:
	.long   tbln
tblm:
	.long   0x01234567
tbln:
	.long   0x00000002
