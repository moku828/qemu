.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R1
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	NEGC    R1, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffffff","sr":"0x700000f1","pc":"0xa0000008"}) */
	
	NEGC    R0, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xffffffff","sr":"0x700000f1","pc":"0xa000000a"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0xFF, R1
	/* t2.stepi() */
	
	MOV     #0xFF, R0
	/* t2.stepi() */
	
	NEGC    R1, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x1","sr":"0x700000f1","pc":"0xa0000012"}) */
	
	NEGC    R0, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x0","sr":"0x700000f1","pc":"0xa0000014"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	NEGC    R1, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x0","sr":"0x700000f0","pc":"0xa000001c"}) */
	
	NEGC    R0, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x0","sr":"0x700000f0","pc":"0xa000001e"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
