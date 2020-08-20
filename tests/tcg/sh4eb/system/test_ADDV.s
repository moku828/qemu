.global test

.section .text, "ax"

test:
	/* test_scenario start */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x7ffffffe, R1
	/* t2.stepi() */
	
	ADDV    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x7fffffff","sr":"0x700000f0","pc":"0xa0000008"}) */
	
	CLRT
	/* t2.stepi() */
	
	MOV     #0x1, R0
	/* t2.stepi() */
	
	MOV.L   immdata_0x7fffffff, R1
	/* t2.stepi() */
	
	ADDV    R0, R1
	/* t2.stepi() */
	/* t2.expect_registers({"r1":"0x80000000","sr":"0x700000f1","pc":"0xa0000010"}) */
	
	/* test_scenario end */
exit:
	BRA     exit
	NOP
	.align  4
immdata_0x7ffffffe:
	.long   0x7FFFFFFE
immdata_0x7fffffff:
	.long   0x7FFFFFFF
