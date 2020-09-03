.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x2, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x1","sr":"0x700000f0","pc":"0xa0000008"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffffff","sr":"0x700000f1","pc":"0xa0000010"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x1, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x0","sr":"0x700000f0","pc":"0xa0000018"}) */
	
	SETT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x2, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x0","sr":"0x700000f0","pc":"0xa0000020"}) */
	
	SETT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x0, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xfffffffe","sr":"0x700000f1","pc":"0xa0000028"}) */
	
	SETT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x1, R1
	/* t2.stepi() */
	
	SUBC    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffffff","sr":"0x700000f1","pc":"0xa0000030"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
