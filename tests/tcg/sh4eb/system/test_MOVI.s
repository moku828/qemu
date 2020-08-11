.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7f"}) */
	
	MOV     #0x80, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffff80"}) */
	
	/* test_scenario end */
