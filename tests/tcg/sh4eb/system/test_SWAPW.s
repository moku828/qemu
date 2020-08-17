.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x01234567, R0
	/* t2.stepi() */
	
	SWAP.W  R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x45670123","pc":"0xa0000004"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x01234567:
	.long   0x01234567
