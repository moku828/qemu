.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV     #0x2, R1
	/* t2.stepi() */
	
	ADD     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x3","pc":"0xa0000006"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
