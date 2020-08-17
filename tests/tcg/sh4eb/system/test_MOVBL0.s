.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R2
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.B   R2, @R1
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   @(R0,R1), R3
	/* t2.stepi() */
	/* t2.expect_registers({"r3":"0x7f","pc":"0xa000000c"}) */
	
	MOV     #0x80, R2
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000004, R1
	/* t2.stepi() */
	
	MOV.B   R2, @R1
	/* t2.stepi() */
	
	MOV     #0x4, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.B   @(R0,R1), R3
	/* t2.stepi() */
	/* t2.expect_registers({"r3":"0xffffff80","pc":"0xa0000018"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x0c000000:
	.long   0x0C000000
immdata_0x0c000004:
	.long   0x0C000004
