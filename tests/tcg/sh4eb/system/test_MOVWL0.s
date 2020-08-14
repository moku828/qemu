.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.W   immdata_0x7fff, R2
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.W   R2, @R1
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.W   @(R0,R1), R3
	/* t2.stepi() */
	/* t2.expect_registers({"r3":"0x7fff"}) */
	
	MOV.W   immdata_0x8000, R2
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.W   R2, @R1
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.W   @(R0,R1), R3
	/* t2.stepi() */
	/* t2.expect_registers({"r3":"0xffff8000"}) */
	
	/* test_scenario end */
immdata_0x7fff:
	.word   0x7FFF
immdata_0x8000:
	.word   0x8000
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
immdata_0x0c000004:
	.long   0x0C000004
