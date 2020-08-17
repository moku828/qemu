.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	MOV.L   immdata_0x7fffffff, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x0c000000, R1
	/* t2.stepi() */
	
	MOV.L   R0, @R1
	/* t2.stepi() */
	
	MOV.L   @R1+, R2
	/* t2.stepi() */
	/* t2.expect_registers({"r2":"0x7fffffff", "r1":"0xc000004","pc":"0xa0000008"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x7fffffff:
	.long   0x7FFFFFFF
immdata_0x0c000000:
	.long   0x0C000000
