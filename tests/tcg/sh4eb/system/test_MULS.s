.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x00000123, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x00000102, R1
	/* t2.stepi() */
	
	MULS.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"macl":"0x12546","pc":"0xa0000006"}) */
	
	MOV.L   immdata_0x0000fedc, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x00000102, R1
	/* t2.stepi() */
	
	MULS.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"macl":"0xfffed9b8","pc":"0xa000000c"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x00000123:
	.long   0x00000123
immdata_0x0000fedc:
	.long   0x0000fedc
immdata_0x00000102:
	.long   0x00000102
