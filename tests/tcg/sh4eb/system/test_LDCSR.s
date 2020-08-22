.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0xffffffff, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700083f3","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0xffffffff:
	.long   0xFFFFFFFF
