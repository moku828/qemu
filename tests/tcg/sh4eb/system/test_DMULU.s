.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x01234567, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x00001000, R1
	/* t2.stepi() */
	
	DMULU.L R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"mach":"0x12","macl":"0x34567000","pc":"0xa0000006"}) */
	
	MOV.L   immdata_0x01234567, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0xfffffffe, R1
	/* t2.stepi() */
	
	DMULU.L R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"mach":"0x1234566","macl":"0xfdb97532","pc":"0xa000000c"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x01234567:
	.long   0x01234567
immdata_0x00001000:
	.long   0x00001000
immdata_0xfffffffe:
	.long   0xFFFFFFFE
