.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	SETT
	/* t2.stepi() */
	
	MOVT    R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x1"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
