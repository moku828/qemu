.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x01234567, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x00010002, R1
	/* t2.stepi() */
	
	MUL.L   R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"macl":"0x47ad8ace","pc":"0xa0000006"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x01234567:
	.long   0x01234567
immdata_0x00010002:
	.long   0x00010002
