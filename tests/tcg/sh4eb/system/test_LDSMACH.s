.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	LDS     R0, MACH
	/* t2.stepi() */
	/* t2.expect_registers({"mach":"0x1","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
