.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x01234567, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x89abcdef, R1
	/* t2.stepi() */
	
	XTRCT   R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x456789ab","pc":"0xa0000006"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x01234567:
	.long   0x01234567
immdata_0x89abcdef:
	.long   0x89abcdef
