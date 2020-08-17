.global test

.section .text, "ax"

test:
	BRA     branch_target_forward
	NOP
	
branch_target_backward:
	BRA     exit
	NOP
	
branch_target_forward:
	BRA     branch_target_backward
	NOP
	
	/* test_scenario start */
	/* t2.stepi() # BRA     branch_target_forward */
	/* t2.expect_registers({"pc":"0xa0000002"}) */
	/* t2.stepi() # NOP */
	/* t2.expect_registers({"pc":"0xa0000008"}) */
	/* t2.stepi() # BRA     branch_target_backward */
	/* t2.expect_registers({"pc":"0xa000000a"}) */
	/* t2.stepi() # NOP */
	/* t2.expect_registers({"pc":"0xa0000004"}) */
	/* test_scenario end */
exit:
	BRA     exit
	NOP
