.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x00007fff, R0
	/* t2.stepi() */
	
	EXTS.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7fff","pc":"0xa0000004"}) */
	
	MOV.L   immdata_0x00008000, R0
	/* t2.stepi() */
	
	EXTS.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0xffff8000","pc":"0xa0000008"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x00007fff:
	.long   0x00007FFF
immdata_0x00008000:
	.long   0x00008000
