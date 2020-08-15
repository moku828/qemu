.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	BRA     branch_target
	/* t2.stepi() */
	
	NOP
	/* t2.stepi() */
	/* t2.expect_registers({"pc":"0xa0000008"}) */
	
	NOP
	
	NOP
	
branch_target:	
	NOP
	
	NOP
	
	/* test_scenario end */
