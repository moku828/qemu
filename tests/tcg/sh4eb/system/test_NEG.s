.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	NEG     R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffffff","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
