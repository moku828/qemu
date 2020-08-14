.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fffffff"}) */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fffffff"}) */
	
	/* test_scenario end */
	.align  4
immdata_0x7fffffff:
	.long   0x7FFFFFFF
