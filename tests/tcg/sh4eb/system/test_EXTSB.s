.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x0000007f, R0
	/* t2.stepi() */
	
	EXTS.B  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7f","pc":"0xa0000004"}) */
	
	MOV.L   immdata_0x00000080, R0
	/* t2.stepi() */
	
	EXTS.B  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffffff80","pc":"0xa0000008"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x0000007f:
	.long   0x0000007F
immdata_0x00000080:
	.long   0x00000080
