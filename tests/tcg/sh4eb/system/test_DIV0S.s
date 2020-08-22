.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	DIV0S   R1, R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700080f2","pc":"0xa000000a"}) */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	MOV     #0xFF, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	DIV0S   R1, R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700081f3","pc":"0xa0000014"}) */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	MOV     #0xFF, R1
	/* t2.stepi() */
	
	DIV0S   R1, R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700082f3","pc":"0xa000001e"}) */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	MOV     #0xFF, R0
	/* t2.stepi() */
	
	MOV     #0xFF, R1
	/* t2.stepi() */
	
	DIV0S   R1, R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700083f2","pc":"0xa0000028"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x700083f3:
	.long   0x700083F3
