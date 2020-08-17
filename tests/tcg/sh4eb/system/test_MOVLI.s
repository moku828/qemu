.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fffffff","pc":"0xa0000002"}) */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x7fffffff","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x7fffffff:
	.long   0x7FFFFFFF
