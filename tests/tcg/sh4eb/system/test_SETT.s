.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	SETT
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700000f1","pc":"0xa0000002"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
