.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0xFF, R0
	/* t2.stepi() */
	
	CMP/PZ  R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f0","pc":"0xa0000006"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	CMP/PZ  R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa000000c"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	CMP/PZ  R0
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000012"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
