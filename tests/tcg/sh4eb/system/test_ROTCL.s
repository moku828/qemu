.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	SETT
	/* t2.stepi() */
	
	MOV.L   immdata_0x12345678, R0
	/* t2.stepi() */
	
	ROTCL   R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x2468acf1","sr":"0x700000f0","pc":"0xa0000006"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV.L   immdata_0x92345678, R0
	/* t2.stepi() */
	
	ROTCL   R0
	/* t2.stepi() */
	/* t2.expect_registers({"r0":"0x2468acf0","sr":"0x700000f1","pc":"0xa000000c"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x12345678:
	.long   0x12345678
immdata_0x92345678:
	.long   0x92345678
