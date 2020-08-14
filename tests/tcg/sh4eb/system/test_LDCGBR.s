.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV     #0x7F, R0
	/* t2.stepi() */
	
	LDC     R0, GBR
	/* t2.stepi() */
	/* t2.expect_registers({"gbr":"0x7f"}) */
	
	/* test_scenario end */
