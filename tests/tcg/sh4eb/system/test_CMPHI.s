.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0xFF, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	CMP/HI  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f0","pc":"0xa0000008"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	CMP/HI  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f0","pc":"0xa0000010"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x0, R0
	/* t2.stepi() */
	
	MOV     #0xFF, R1
	/* t2.stepi() */
	
	CMP/HI  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000018"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
