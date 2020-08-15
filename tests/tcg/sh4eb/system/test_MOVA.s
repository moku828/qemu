.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOVA    @(8, PC), R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa0000008"}) */
	
	MOVA    @(6, PC), R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xa0000008"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.long   0x7FFFFFFF
