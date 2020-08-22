.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0xffff8000, R0
	/* t2.stepi() */
	
	EXTU.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x8000","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0xffff8000:
	.long   0xFFFF8000
