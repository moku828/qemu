.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.W   immdata_0x7fff, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.W   R0, @R1
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.W   @(4, R1), R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fff","pc":"0xa000000a"}) */
	
	MOV.W   immdata_0x8000, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.W   R0, @R1
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.W   @(4, R1), R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xffff8000","pc":"0xa0000014"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
immdata_0x0c000004:
	.long   0x0C000004
