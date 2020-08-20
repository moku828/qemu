.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	ADD     #0x2, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x3","pc":"0xa0000004"}) */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	ADD     #0x82, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0xffffff83","pc":"0xa0000008"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
