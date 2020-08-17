.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7f","pc":"0xa0000002"}) */
	
	MOV     #0x80, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xffffff80","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
