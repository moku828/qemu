.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x00020000, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x00000006, R1
	/* t2.stepi() */
	
	DIV0U
	/* t2.stepi() */
	
	.rept   16
	DIV1    R0, R1
	.endr
	/* for i in range (16): */
	/* 	t2.stepi() */
	
	ROTCL   R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x3","pc":"0xa0000028"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x00020000:
	.long   0x00020000
immdata_0x00000006:
	.long   0x00000006
