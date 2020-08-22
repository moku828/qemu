.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x700083f3, R0
	/* t2.stepi() */
	
	LDC     R0, SR
	/* t2.stepi() */
	
	DIV0U
	/* t2.stepi() */
	/* t2.expect_registers({"sr":"0x700080f2","pc":"0xa0000006"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x700083f3:
	.long   0x700083F3
