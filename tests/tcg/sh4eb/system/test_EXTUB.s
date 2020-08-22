.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0xffffff80, R0
	/* t2.stepi() */
	
	EXTU.B  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x80","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0xffffff80:
	.long   0xFFFFFF80
