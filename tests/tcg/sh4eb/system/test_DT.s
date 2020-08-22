.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	DT      R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x0","sr":"0x700000f1","pc":"0xa0000006"}) */
	
	SETT
	/* t2.stepi() */
	
	MOV     #0x2, R0
	/* t2.stepi() */
	
	DT      R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x1","sr":"0x700000f0","pc":"0xa000000c"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
